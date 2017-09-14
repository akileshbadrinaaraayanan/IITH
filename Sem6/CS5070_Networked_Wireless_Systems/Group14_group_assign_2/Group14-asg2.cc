/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */
#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
 #include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/applications-module.h"
 #include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-module.h"
 #include "ns3/config-store.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/config-store-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("LenaX2HandoverExample");

void printStats(FlowMonitorHelper &flowmon_helper, bool perFlowInfo, std::string schedType, double interPacketInterval, double speed, std::string handAlgo, double hysteresis, int celloffset)
{
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon_helper.GetClassifier());
    std::string proto;
    Ptr<FlowMonitor> monitor = flowmon_helper.GetMonitor ();
    std::map < FlowId, FlowMonitor::FlowStats > stats = monitor->GetFlowStats();
    double totalTimeReceiving;
    uint64_t totalPacketsReceived, totalPacketsDropped, totalBytesReceived;

    totalBytesReceived = 0, totalPacketsDropped = 0, totalPacketsReceived = 0, totalTimeReceiving = 0;
    double total_throughput=0;
    int n=0;
    for (std::map< FlowId, FlowMonitor::FlowStats>::iterator flow = stats.begin(); flow != stats.end(); flow++)
      { n++;
        int arj=1;
        while(arj--){
          Ipv4FlowClassifier::FiveTuple  t = classifier->FindFlow(flow->first);
          switch(t.protocol)
           {
           case(6):
               proto = "TCP";
               break;
           case(17):
               proto = "UDP";
               std::cout << "UDP" << std::endl;
               break;
           default:
               exit(1);
           }
           totalBytesReceived += (double) flow->second.rxBytes * 8;
           totalTimeReceiving += flow->second.timeLastRxPacket.GetSeconds ();
           totalPacketsReceived += flow->second.rxPackets;
           totalPacketsDropped += flow->second.txPackets - flow->second.rxPackets;
           if (perFlowInfo) {
             std::cout << "FlowID: " << flow->first << " (" << proto << " "
                       << t.sourceAddress << " / " << t.sourcePort << " --> "
                       << t.destinationAddress << " / " << t.destinationPort << ")" << std::endl;
             std::cout << "  Tx Bytes: " << flow->second.txBytes << std::endl;
             std::cout << "  Rx Bytes: " << flow->second.rxBytes << std::endl;
             std::cout << "  Tx Packets: " << flow->second.txPackets << std::endl;
             std::cout << "  Rx Packets: " << flow->second.rxPackets << std::endl;
             std::cout << "  Time LastRxPacket: " << flow->second.timeLastRxPacket.GetSeconds () << "s" << std::endl;
             std::cout << "  Lost Packets: " << flow->second.lostPackets << std::endl;
             std::cout << "  Pkt Lost Ratio: " << ((double)flow->second.txPackets-(double)flow->second.rxPackets)/(double)flow->second.txPackets << std::endl;
             std::cout << "  Throughput: " << ( ((double)flow->second.rxBytes*8) / (flow->second.timeLastRxPacket.GetSeconds ()) ) << "bps" << std::endl;
             total_throughput+= ( ((double)flow->second.rxBytes*8) / (flow->second.timeLastRxPacket.GetSeconds ()) );
             std::cout << "  Mean{Delay}: " << (flow->second.delaySum.GetSeconds()/flow->second.rxPackets) << std::endl;
             std::cout << "  Mean{Jitter}: " << (flow->second.jitterSum.GetSeconds()/(flow->second.rxPackets)) << std::endl;
           }
       }

     }
     std::cout << speed << "\t" << interPacketInterval << "\t" << schedType << "\t"  << handAlgo << "\t" << std::endl;
     std::cout << hysteresis << "\t" << celloffset << std::endl;
     std::cout<<"Average throughput= "<<total_throughput/n<<" bps\n";
}


void
NotifyConnectionEstablishedUe (std::string context,
                               uint64_t imsi,
                               uint16_t cellid,
                               uint16_t rnti)
{
  std::cout << Simulator::Now ().GetSeconds () << " " << context
            << " UE IMSI " << imsi
            << ": connected to CellId " << cellid
            << " with RNTI " << rnti
            << std::endl;
}

void
NotifyHandoverStartUe (std::string context,
                       uint64_t imsi,
                       uint16_t cellid,
                       uint16_t rnti,
                       uint16_t targetCellId)
{
  std::cout << Simulator::Now ().GetSeconds () << " " << context
            << " UE IMSI " << imsi
            << ": previously connected to CellId " << cellid
            << " with RNTI " << rnti
            << ", doing handover to CellId " << targetCellId
            << std::endl;
}

void
NotifyHandoverEndOkUe (std::string context,
                       uint64_t imsi,
                       uint16_t cellid,
                       uint16_t rnti)
{
  std::cout << Simulator::Now ().GetSeconds () << " " << context
            << " UE IMSI " << imsi
            << ": successful handover to CellId " << cellid
            << " with RNTI " << rnti
            << std::endl;
}

void
NotifyConnectionEstablishedEnb (std::string context,
                                uint64_t imsi,
                                uint16_t cellid,
                                uint16_t rnti)
{
  std::cout << Simulator::Now ().GetSeconds () << " " << context
            << " eNB CellId " << cellid
            << ": successful connection of UE with IMSI " << imsi
            << " RNTI " << rnti
            << std::endl;
}

void
NotifyHandoverStartEnb (std::string context,
                        uint64_t imsi,
                        uint16_t cellid,
                        uint16_t rnti,
                        uint16_t targetCellId)
{
  std::cout << Simulator::Now ().GetSeconds () << " " << context
            << " eNB CellId " << cellid
            << ": start handover of UE with IMSI " << imsi
            << " RNTI " << rnti
            << " to CellId " << targetCellId
            << std::endl;
}

void
NotifyHandoverEndOkEnb (std::string context,
                        uint64_t imsi,
                        uint16_t cellid,
                        uint16_t rnti)
{
  std::cout << Simulator::Now ().GetSeconds () << " " << context
            << " eNB CellId " << cellid
            << ": completed handover of UE with IMSI " << imsi
            << " RNTI " << rnti
            << std::endl;
}


/**
 * Sample simulation script for a X2-based handover.
 * It instantiates two eNodeB, attaches one UE to the 'source' eNB and
 * triggers a handover of the UE towards the 'target' eNB.
 */
int
main (int argc, char *argv[])
{
  // LogLevel logLevel = (LogLevel)(LOG_PREFIX_FUNC | LOG_PREFIX_TIME | LOG_LEVEL_ALL);

  // LogComponentEnable ("LteHelper", logLevel);
  // LogComponentEnable ("EpcHelper", logLevel);
  // LogComponentEnable ("EpcEnbApplication", logLevel);
  // LogComponentEnable ("EpcX2", logLevel);
  // LogComponentEnable ("EpcSgwPgwApplication", logLevel);

  // LogComponentEnable ("LteEnbRrc", logLevel);
  // LogComponentEnable ("LteEnbNetDevice", logLevel);
  // LogComponentEnable ("LteUeRrc", logLevel);
  // LogComponentEnable ("LteUeNetDevice", logLevel);
  Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue(20.0));
  uint16_t numberOfEnbs = 8;
  uint16_t numberOfUes = 3;
  double simTime = 10.0;
  double distance = 250.0;
  double interPacketInterval = 10;
  std::string schedType = "ns3::PfFfMacScheduler";
  std::string handAlgo = "ns3::A3RsrpHandoverAlgorithm";
  double hysteresis = 1.0;
  int celloffset = 1;
  double speed = 20.0;

  //uint16_t numberOfUes = 1;
  //uint16_t numberOfEnbs = 2;
 // uint16_t numBearersPerUe = 2;
  //uint16_t numBearersPerUe = 1;
  //double simTime = 0.300;
  //double distance = 100.0;

  // change some default attributes so that they are reasonable for
  // this scenario, but do this before processing command line
  // arguments, so that the user is allowed to override these settings
  //Config::SetDefault ("ns3::UdpClient::Interval", TimeValue (MilliSeconds (10)));
  //Config::SetDefault ("ns3::UdpClient::MaxPackets", UintegerValue (1000000));
  //Config::SetDefault ("ns3::LteHelper::UseIdealRrc", BooleanValue (false));

  // Command line arguments
  CommandLine cmd;
  cmd.AddValue ("numberOfUes", "Number of UEs", numberOfUes);
  cmd.AddValue ("numberOfEnbs", "Number of eNodeBs", numberOfEnbs);
  cmd.AddValue ("simTime", "Total duration of the simulation (in seconds)", simTime);
  cmd.AddValue("distance", "Distance between eNBs [m]", distance);
  cmd.AddValue("schedType","Type of Scheduler",schedType);
  cmd.AddValue("interPacketInterval", "Inter packet interval [ms])", interPacketInterval);
  cmd.AddValue("speed","UE speed",speed);
  cmd.AddValue("handAlgo", "Type of handover algo", handAlgo);
  cmd.AddValue("hysteresis", "Value of hysteresis", hysteresis);
  cmd.AddValue("celloffset", "Cell offset", celloffset);
  cmd.Parse (argc, argv);

  NodeContainer ueNodes[8]; // 8 containers of 4 UE nodes each.
  NodeContainer enbNodes;

  enbNodes.Create(numberOfEnbs);
  
  for(uint16_t i=0; i<8; i++){
    ueNodes[i].Create(numberOfUes);
  }


  //Placing eNB
  Ptr<ListPositionAllocator> enbpositionAlloc = CreateObject<ListPositionAllocator> ();

  enbpositionAlloc->Add (Vector(0, 0, 0));

  //Placing eNB2
  enbpositionAlloc->Add (Vector(250, 0, 0));
  enbpositionAlloc->Add (Vector(500, 0, 0));
  enbpositionAlloc->Add (Vector(750, 0, 0));
  //Placing eNB3
  enbpositionAlloc->Add (Vector(750, 250, 0));
  enbpositionAlloc->Add (Vector(500, 250, 0));
  enbpositionAlloc->Add (Vector(250, 250, 0));
  //Placing eNB4
  enbpositionAlloc->Add (Vector(0,250, 0));

  char spd[100];
  sprintf(spd,"ns3::ConstantRandomVariable[Constant=%f]",speed);

    //Install Mobility Model
  MobilityHelper mobility;
 
 
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator(enbpositionAlloc);
  mobility.Install(enbNodes);

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Mode", StringValue ("Time"),
                             "Time", StringValue ("2s"),
                             "Speed", StringValue (spd),
                             "Bounds", StringValue ("-2000|2000|-2000|2000"));

mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("0.0"),
                                 "Y", StringValue ("0.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=100|Max=125]"));

mobility.Install(ueNodes[0]);


mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("250.0"),
                                 "Y", StringValue ("0.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=100|Max=125]"));

mobility.Install(ueNodes[1]);

mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("500.0"),
                                 "Y", StringValue ("0.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=100|Max=125]"));

mobility.Install(ueNodes[2]);

mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("750.0"),
                                 "Y", StringValue ("0.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=100|Max=125]"));

mobility.Install(ueNodes[3]);

mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("750.0"),
                                 "Y", StringValue ("250.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=100|Max=125]"));

mobility.Install(ueNodes[4]);


mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("500.0"),
                                 "Y", StringValue ("250.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=100|Max=125]"));

mobility.Install(ueNodes[5]);

mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("250.0"),
                                 "Y", StringValue ("250.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=100|Max=125]"));

mobility.Install(ueNodes[6]);

mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("0.0"),
                                 "Y", StringValue ("250.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=100|Max=125]"));

mobility.Install(ueNodes[7]);


  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper> ();
  
  lteHelper->SetEnbDeviceAttribute ("DlBandwidth", UintegerValue (50));
  lteHelper->SetEnbDeviceAttribute ("UlBandwidth", UintegerValue (50));

  lteHelper->SetEpcHelper (epcHelper);
 // lteHelper->SetHandoverAlgorithmType ("ns3::NoOpHandoverAlgorithm"); // disable automatic handover
  lteHelper->SetSchedulerType(schedType);
  lteHelper->SetHandoverAlgorithmType (handAlgo);
  /*if(strcmp(handAlgo, "RSRP") == 0) {
    lteHelper->SetHandoverAlgorithmType ("ns3::A3RsrpHandoverAlgorithm");
    //lteHelper->SetHandoverAlgorithmAttribute ("Hysteresis",DoubleValue (hys)); 
  }
  
  else {
    lteHelper->SetHandoverAlgorithmType("ns3::A2A4RsrqHandoverAlgorithm");
    //lteHelper->SetHandoverAlgorithmAttribute ("NeighbourCellOffset",UintegerValue (celloffset));
  }*/
  if(handAlgo.compare("ns3::A3RsrpHandoverAlgorithm") == 0)
    {
        //lteHelper->SetHandoverAlgorithmType ("ns3::A3RsrpHandoverAlgorithm");
        std::cout << "hysteresis set " << std::endl;   
        lteHelper->SetHandoverAlgorithmAttribute ("Hysteresis", DoubleValue (hysteresis));
    }
    else if(handAlgo.compare("ns3::A2A4RsrqHandoverAlgorithm") == 0)
    {
      std::cout << "cell offset set " << std::endl;
      //lteHelper->SetHandoverAlgorithmType (handAlgo);	
      //lteHelper->SetHandoverAlgorithmType("ns3::A2A4RsrqHandoverAlgorithm");
      lteHelper->SetHandoverAlgorithmAttribute ("NeighbourCellOffset",UintegerValue (celloffset));   
    }
    

  Ptr<Node> pgw = epcHelper->GetPgwNode ();
  Ptr<ListPositionAllocator> pgwpositionAlloc = CreateObject<ListPositionAllocator> ();
  pgwpositionAlloc->Add(Vector(30,30,0));
  mobility.SetPositionAllocator(pgwpositionAlloc);
  mobility.Install(pgw);

  // Create a single RemoteHost
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

    Ptr<ListPositionAllocator> rHpositionAlloc = CreateObject<ListPositionAllocator> ();
  rHpositionAlloc->Add(Vector(0,0,0));
  mobility.SetPositionAllocator(rHpositionAlloc);
  mobility.Install(remoteHostContainer);

  // Create the Internet
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("1Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);

  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  //Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);


  // Routing of the Internet Host (towards the LTE network)
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  // interface 0 is localhost, 1 is the p2p device
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  /*NodeContainer ueNodes;
  NodeContainer enbNodes;
  enbNodes.Create (numberOfEnbs);
  ueNodes.Create (numberOfUes);

  // Install Mobility Model
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  for (uint16_t i = 0; i < numberOfEnbs; i++)
    {
      positionAlloc->Add (Vector (distance * 2 * i - distance, 0, 0));
    }
  for (uint16_t i = 0; i < numberOfUes; i++)
    {
      positionAlloc->Add (Vector (0, 0, 0));
    }
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (enbNodes);
  mobility.Install (ueNodes);
*/
  // Install LTE Devices in eNB and UEs
  NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);
  //NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ueNodes);

  NetDeviceContainer ueLteDevs[8];
  for(uint16_t j=0;j<8;j++)
    ueLteDevs[j] = lteHelper->InstallUeDevice (ueNodes[j]);

   Ipv4InterfaceContainer ueIpIface[8];
  // Install the IP stack on the UEs
  for(uint16_t j=0;j<8;j++){
    internet.Install (ueNodes[j]);
    
    ueIpIface[j] = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs[j]));
  // Assign IP address to UEs, and install applications
  
    for (uint32_t u = 0; u < ueNodes[j].GetN (); ++u)
      {
        Ptr<Node> ueNode = ueNodes[j].Get (u);
        // Set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
        ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
      }

    // Attach all UEs to eNodeB  
    for (uint16_t i = 0; i < numberOfUes; i++)
        {
          lteHelper->Attach (ueLteDevs[j].Get(i), enbLteDevs.Get(j));
          // side effect: the default EPS bearer will be activated
        }
    }

  // Install the IP stack on the UEs
  /*internet.Install (ueNodes);
  Ipv4InterfaceContainer ueIpIfaces;
  ueIpIfaces = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs));
  // Assign IP address to UEs, and install applications
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      Ptr<Node> ueNode = ueNodes.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }


  // Attach all UEs to the first eNodeB
  for (uint16_t i = 0; i < numberOfUes; i++)
    {
      lteHelper->Attach (ueLteDevs.Get (i), enbLteDevs.Get (0));
    }
*/


  NS_LOG_LOGIC ("setting up applications");

  // Install and start applications on UEs and remote host
  uint16_t dlPort = 10000;
  uint16_t ulPort = 20000;
   ApplicationContainer clientApps;
   ApplicationContainer serverApps;
  // randomize a bit start times to avoid simulation artifacts
  // (e.g., buffer overflows due to packet transmissions happening
  // exactly at the same time)
  Ptr<UniformRandomVariable> startTimeSeconds = CreateObject<UniformRandomVariable> ();
  startTimeSeconds->SetAttribute ("Min", DoubleValue (0));
  startTimeSeconds->SetAttribute ("Max", DoubleValue (0.010));

  for (uint32_t u = 0; u < 24; ++u)
    {
      //Ptr<Node> ue = ueNodes.Get (u);
      // Set the default gateway for the UE
      //Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ue->GetObject<Ipv4> ());
      //ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);

          ++dlPort;
          ++ulPort;

         

           PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
          PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));

          serverApps.Add (dlPacketSinkHelper.Install (ueNodes[u/3].Get(u%3)));
          serverApps.Add (ulPacketSinkHelper.Install (remoteHost));
         // serverApps.Add (packetSinkHelper.Install (ueNodes[u/3].Get(u%3)));

      
          UdpClientHelper dlClient (ueIpIface[u/3].GetAddress (u%3), dlPort);
          dlClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
          dlClient.SetAttribute ("MaxPackets", UintegerValue(1000000));
          dlClient.SetAttribute("PacketSize",UintegerValue(1500));
          
           clientApps.Add (dlClient.Install (remoteHost));
            /*NS_LOG_LOGIC ("installing UDP DL app for UE " << u);
          UdpClientHelper dlClientHelper (ueIpIfaces.GetAddress (u), dlPort);
          clientApps.Add (dlClientHelper.Install (remoteHost));
          PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
          serverApps.Add (dlPacketSinkHelper.Install (ue));

          NS_LOG_LOGIC ("installing UDP UL app for UE " << u);
          UdpClientHelper ulClientHelper (remoteHostAddr, ulPort);
          clientApps.Add (ulClientHelper.Install (ue));
          PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory",
                                               InetSocketAddress (Ipv4Address::GetAny (), ulPort));
          serverApps.Add (ulPacketSinkHelper.Install (remoteHost));
        */

          Ptr<EpcTft> tft = Create<EpcTft> ();
          EpcTft::PacketFilter dlpf;
          dlpf.localPortStart = dlPort;
          dlpf.localPortEnd = dlPort;
          tft->Add (dlpf);
          EpcTft::PacketFilter ulpf;
          ulpf.remotePortStart = ulPort;
          ulpf.remotePortEnd = ulPort;
          tft->Add (ulpf);
          EpsBearer bearer (EpsBearer::NGBR_VIDEO_TCP_DEFAULT);
          lteHelper->ActivateDedicatedEpsBearer (ueLteDevs[u/3].Get (u%3), bearer, tft);

          Time startTime = Seconds (startTimeSeconds->GetValue ());
          serverApps.Start (startTime);
          clientApps.Start (startTime);

        
    }

  serverApps.Start (Seconds (0.01));
  clientApps.Start (Seconds (0.01)); 
  // Add X2 inteface
  lteHelper->AddX2Interface (enbNodes);

  // X2-based Handover
  //lteHelper->HandoverRequest (Seconds (0.100), ueLteDevs.Get (0), enbLteDevs.Get (0), enbLteDevs.Get (1));

  // Uncomment to enable PCAP tracing
  //p2ph.EnablePcapAll("lena-x2-handover");

  lteHelper->EnablePhyTraces ();
  lteHelper->EnableMacTraces ();
  lteHelper->EnableRlcTraces ();
  lteHelper->EnablePdcpTraces ();
  Ptr<RadioBearerStatsCalculator> rlcStats = lteHelper->GetRlcStats ();
  rlcStats->SetAttribute ("EpochDuration", TimeValue (Seconds (0.05)));
  Ptr<RadioBearerStatsCalculator> pdcpStats = lteHelper->GetPdcpStats ();
  pdcpStats->SetAttribute ("EpochDuration", TimeValue (Seconds (0.05)));


  // connect custom trace sinks for RRC connection establishment and handover notification
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/ConnectionEstablished",
                   MakeCallback (&NotifyConnectionEstablishedEnb));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/ConnectionEstablished",
                   MakeCallback (&NotifyConnectionEstablishedUe));
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverStart",
                   MakeCallback (&NotifyHandoverStartEnb));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverStart",
                   MakeCallback (&NotifyHandoverStartUe));
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverEndOk",
                   MakeCallback (&NotifyHandoverEndOkEnb));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverEndOk",
                   MakeCallback (&NotifyHandoverEndOkUe));

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> Monitor;

  for(uint16_t j = 0; j < 8; j++)
  {
    Monitor = flowmon.Install(ueNodes[j]);
  }
    Monitor = flowmon.Install(remoteHostContainer);
  Simulator::Stop(Seconds(simTime));

  //Simulator::Stop (Seconds (simTime));
  Simulator::Run ();
  printStats(flowmon,true, schedType, interPacketInterval, speed, handAlgo, hysteresis, celloffset);
  // GtkConfigStore config;
  // config.ConfigureAttributes ();

  Simulator::Destroy ();
  return 0;

}
