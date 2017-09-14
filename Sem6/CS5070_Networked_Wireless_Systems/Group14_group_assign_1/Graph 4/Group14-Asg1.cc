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
#include "ns3/config-store.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/netanim-module.h"
//#include "ns3/visualizer-module.h"//Require for visualization

using namespace ns3;

/**
 * Sample simulation script for LTE + EPC. It instantiates several eNodeB,
 * attaches one UE per eNodeB starts a flow for each UE to  and from a remote host.
 * It also  starts yet another flow between each UE pair.
 */
NS_LOG_COMPONENT_DEFINE ("EpcFirstExample");

void printStats(FlowMonitorHelper &flowmon_helper, bool perFlowInfo, std::string schedType, double interPacketInterval, double speed)
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
     std::cout << speed << "\t" << interPacketInterval << "\t" << schedType << "\t"  << std::endl;
    
     std::cout<<"Average throughput= "<<total_throughput/n<<" bps\n";
}

int main (int argc, char *argv[])
{
  
  Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue(30.0));
  uint16_t numberOfeNodeBs = 4;
  uint16_t numberOfUEs = 5;
  double simTime = 10.0;
  double distance = 1000.0;
  double interPacketInterval = 1;
  std::string schedType = "ns3::PfFfMacScheduler";
  double speed = 0.0;
  //double radius=500.0;
  
  //{ PyViz v; }//Require for visualization
  // Command line arguments
  CommandLine cmd;
  cmd.AddValue("numberOfNodes", "Number of eNodeBs + UE pairs", numberOfeNodeBs);
  cmd.AddValue("simTime", "Total duration of the simulation [s])", simTime);
  cmd.AddValue("distance", "Distance between eNBs [m]", distance);
  cmd.AddValue("schedType","Type of Scheduler",schedType);
  cmd.AddValue("interPacketInterval", "Inter packet interval [ms])", interPacketInterval);
  cmd.AddValue("speed","UE speed",speed);
  cmd.Parse(argc, argv);

  NodeContainer ueNodes[4]; // 4 containers of 5 UE nodes each.
  NodeContainer enbNodes;

  enbNodes.Create(numberOfeNodeBs);
  
  for(uint16_t i=0;i<4;i++){
    ueNodes[i].Create(numberOfUEs);
  }


  //Placing eNB
  Ptr<ListPositionAllocator> enbpositionAlloc = CreateObject<ListPositionAllocator> ();

  enbpositionAlloc->Add (Vector(0, 0, 0));

  //Placing eNB2
  enbpositionAlloc->Add (Vector(1000, 0, 0));

  //Placing eNB3
  enbpositionAlloc->Add (Vector(1000, 1000, 0));

  //Placing eNB4
  enbpositionAlloc->Add (Vector(0,1000, 0));

/*
  //Placing UEs
  Ptr<ListPositionAllocator> uepositionAlloc = CreateObject<ListPositionAllocator> ();
  uepositionAlloc->Add (Vector(0, 0, 0));
  uepositionAlloc->Add (Vector(1000, 0, 0));
  uepositionAlloc->Add (Vector(1000, 1000, 0));
  uepositionAlloc->Add (Vector(0,1000, 0));
  //uepositionAlloc->Add (Vector(100, 140, 0));
*/

char spd[100];
sprintf(spd,"ns3::ConstantRandomVariable[Constant=%f]",speed);
 

  //Install Mobility Model
  MobilityHelper mobility;
 
 
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator(enbpositionAlloc);
  mobility.Install(enbNodes);
  //mobility.SetPositionAllocator(uepositionAlloc);
  //mobility.Install(ueNodes);


 /* mobility.SetPositionAllocator
  ("ns3::RandomDiscPositionAllocator",
   "X", StringValue ("0.0"),
   "Y", StringValue ("0.0"),
   "Rho", radius
  ));

  mobility.SetPositionAllocator
  ("ns3::RandomDiscPositionAllocator",
   "X", StringValue ("0.0"),
   "Y", StringValue ("1000.0"),
   "Rho", radius
  ));

  mobility.SetPositionAllocator
  ("ns3::RandomDiscPositionAllocator",
   "X", StringValue ("1000.0"),
   "Y", StringValue ("1000.0"),
   "Rho", radius
  ));

  mobility.SetPositionAllocator
  ("ns3::RandomDiscPositionAllocator",
   "X", StringValue ("0.0"),
   "Y", StringValue ("1000.0"),
   "Rho", radius
  ));
*/
mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Mode", StringValue ("Time"),
                             "Time", StringValue ("2s"),
                             "Speed", StringValue (spd),
                             "Bounds", StringValue ("-500|1500|-500|1500"));


mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("0.0"),
                                 "Y", StringValue ("0.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=0|Max=500]"));

mobility.Install(ueNodes[0]);


mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("1000.0"),
                                 "Y", StringValue ("0.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=0|Max=500]"));

mobility.Install(ueNodes[1]);

mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("1000.0"),
                                 "Y", StringValue ("1000.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=0|Max=500]"));

mobility.Install(ueNodes[2]);

mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue ("0.0"),
                                 "Y", StringValue ("1000.0"),
                                 "Rho", StringValue ("ns3::UniformRandomVariable[Min=0|Max=500]"));

mobility.Install(ueNodes[3]);

  //Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (30));

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  Ptr<PointToPointEpcHelper>  epcHelper = CreateObject<PointToPointEpcHelper> ();

  lteHelper->SetEnbDeviceAttribute ("DlBandwidth", UintegerValue (50));
  lteHelper->SetEnbDeviceAttribute ("UlBandwidth", UintegerValue (50));

  lteHelper->SetEpcHelper (epcHelper);
  //lteHelper->SetSchedulerType("ns3::FdBetFfMacScheduler");
  lteHelper->SetSchedulerType(schedType);


  Ptr<Node> pgw = epcHelper->GetPgwNode ();
  Ptr<ListPositionAllocator> pgwpositionAlloc = CreateObject<ListPositionAllocator> ();
  pgwpositionAlloc->Add(Vector(30,30,0));
  mobility.SetPositionAllocator(pgwpositionAlloc);
  mobility.Install(pgw);

   // Create a single RemoteHost
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create(1);
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
  // interface 0 is localhost, 1 is the p2p device
  //Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1); //uncomment the below line for uplink of client

  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  // Install LTE Devices to the nodes
  NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);

  NetDeviceContainer ueLteDevs[4];
  for(uint16_t j=0;j<4;j++)
    ueLteDevs[j] = lteHelper->InstallUeDevice (ueNodes[j]);

  Ipv4InterfaceContainer ueIpIface[4];
  // Install the IP stack on the UEs
  for(uint16_t j=0;j<4;j++){
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
    for (uint16_t i = 0; i < numberOfUEs; i++)
        {
          lteHelper->Attach (ueLteDevs[j].Get(i), enbLteDevs.Get(j));
          // side effect: the default EPS bearer will be activated
        }
    }

  // Install and start applications on UEs and remote host
  uint16_t dlPort = 1234;
  uint16_t ulPort = 2000;
  uint16_t otherPort = 3000;
  ApplicationContainer clientApps;
  ApplicationContainer serverApps;
  
  for (uint32_t u = 0; u < 20; ++u)
    {
      ++ulPort;
      ++otherPort;
      PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
      PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
      PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), otherPort));
      //serverApps.Add (dlPacketSinkHelper.Install (ueNodes.Get(u)));
      serverApps.Add (dlPacketSinkHelper.Install (ueNodes[u/5].Get(u%5)));
      serverApps.Add (ulPacketSinkHelper.Install (remoteHost));
      serverApps.Add (packetSinkHelper.Install (ueNodes[u/5].Get(u%5)));

      
      UdpClientHelper dlClient (ueIpIface[u/5].GetAddress (u%5), dlPort);
      dlClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
      dlClient.SetAttribute ("MaxPackets", UintegerValue(1000));
      dlClient.SetAttribute("PacketSize",UintegerValue(1500));

      //uncomment below lines for uplink of client
      //UdpClientHelper ulClient (remoteHostAddr, ulPort);
      //ulClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
      //ulClient.SetAttribute ("MaxPackets", UintegerValue(1000));
      //ulClient.SetAttribute("PacketSize",UintegerValue(1024));

      UdpClientHelper client (ueIpIface[u/5].GetAddress (u%5), otherPort);
      client.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
      client.SetAttribute ("MaxPackets", UintegerValue(1000));
      client.SetAttribute("PacketSize",UintegerValue(1500));

      clientApps.Add (dlClient.Install (remoteHost));
      //uncomment the below line for uplink of client
      //clientApps.Add (ulClient.Install (ueNodes.Get(u)));
      /*if (u+1 < ueNodes.GetN ())
        {
          clientApps.Add (client.Install (ueNodes.Get(u+1)));
        }
      else
        {
          clientApps.Add (client.Install (ueNodes.Get(0)));
        }*/

/*
      ++ulPort;
      ++otherPort;
      PacketSinkHelper dlPacketSinkHelper1("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
      PacketSinkHelper ulPacketSinkHelper1("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
      PacketSinkHelper packetSinkHelper1("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), otherPort));
      serverApps.Add (dlPacketSinkHelper1.Install (ueNodes.Get(u)));
      serverApps.Add (ulPacketSinkHelper1.Install (remoteHost));
      serverApps.Add (packetSinkHelper1.Install (ueNodes.Get(u)));


      UdpClientHelper dlClient1 (ueIpIface.GetAddress (u), dlPort);
      dlClient1.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
      dlClient1.SetAttribute ("MaxPackets", UintegerValue(1000));
      dlClient1.SetAttribute("PacketSize",UintegerValue(1024));

      //uncomment below lines for uplink of client
      //UdpClientHelper ulClient (remoteHostAddr, ulPort);
      //ulClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
      //ulClient.SetAttribute ("MaxPackets", UintegerValue(1000));
      //ulClient.SetAttribute("PacketSize",UintegerValue(1024));

      UdpClientHelper client1 (ueIpIface.GetAddress (u), otherPort);
      client1.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
      client1.SetAttribute ("MaxPackets", UintegerValue(2000));
      client1.SetAttribute("PacketSize",UintegerValue(1024));

      clientApps.Add (dlClient1.Install (remoteHost));

      */


    }
  serverApps.Start (Seconds (0.01));
  clientApps.Start (Seconds (0.01));
  lteHelper->EnableTraces ();

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> Monitor;

  for(uint16_t j = 0; j < 4; j++)
  {
    Monitor = flowmon.Install(ueNodes[j]);
  }
    Monitor = flowmon.Install(remoteHostContainer);
  Simulator::Stop(Seconds(simTime));
  /*
  AnimationInterface::SetNodeDescription(remoteHostContainer.Get (0),"RemoteHost");
  AnimationInterface::SetNodeDescription(pgw,"PacketGateway");
  AnimationInterface::SetNodeDescription (enbNodes.Get(0), "ENB");
  AnimationInterface::SetNodeDescription (ueNodes.Get(0), "UE1");
  AnimationInterface::SetNodeDescription (ueNodes.Get(1), "UE2");
  AnimationInterface::SetNodeDescription (ueNodes.Get(2), "UE3");
  AnimationInterface::SetNodeDescription (ueNodes.Get(3), "UE4");
  AnimationInterface::SetNodeDescription (ueNodes.Get(4), "UE5");
  AnimationInterface::SetNodeColor(remoteHostContainer,0,255,0);
  AnimationInterface::SetNodeColor(pgw,0,255,0);
  AnimationInterface::SetNodeColor (enbNodes, 255, 0, 0);
  AnimationInterface::SetNodeColor (ueNodes, 0, 0, 255);

  AnimationInterface anim ("lena-simple-epc-animation.xml"); // Mandatory
  */
  Simulator::Run();
  printStats(flowmon,true, schedType, interPacketInterval, speed);
  Simulator::Destroy();
  return 0;

}
