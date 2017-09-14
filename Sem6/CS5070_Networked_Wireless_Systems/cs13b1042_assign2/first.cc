/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */
#include "ns3/netanim-module.h" //Change by Akilesh B
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  //LogComponentEnable ("MyFirstScriptExample", LOG_ALL);

  uint32_t nPackets=1;
  std::string errorModelType = "ns3::RateErrorModel";

  CommandLine cmd;
  cmd.AddValue("nPackets", "number of packets to send by echoClient", nPackets);
  cmd.AddValue("errorModelType", "TypeId of the error model to use", errorModelType);
  cmd.Parse(argc, argv);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("10Kbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  Ptr<UniformRandomVariable> v = CreateObject<UniformRandomVariable> ();
  v->SetAttribute ("Min", DoubleValue (0.0));
  v->SetAttribute ("Max", DoubleValue (1.0));


  Ptr<UniformRandomVariable> v1 = CreateObject<UniformRandomVariable> ();
  v1->SetAttribute ("Min", DoubleValue (1.0));
  v1->SetAttribute ("Max", DoubleValue (5.0));


  if(errorModelType.compare("RateErrorModel") == 0)
  {
	  Ptr<RateErrorModel> em = CreateObject<RateErrorModel> ();
  //em->SetAttribute ("ErrorRate", DoubleValue (0.00001));
  //em->SetAttribute("ErrorUnit", StringValue("ERROR_UNIT_BIT"));
  //em->SetAttribute("RanVar", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=1.0]"));

  em->SetRate(0.1);
  em->SetUnit((RateErrorModel::ErrorUnit)2);
  em->SetRandomVariable(v);

  devices.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
  devices.Get (0)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
  }
  else if(errorModelType.compare("BurstErrorModel") == 0)
  	 {
	  //std::cout<< "inside burst error model" << std::endl;
	  Ptr<BurstErrorModel> em = CreateObject<BurstErrorModel> ();

	  em->SetRandomBurstSize(v1);
	  em->SetRandomVariable(v);
	  em->SetBurstRate(0.4);


    devices.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
    devices.Get (0)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
   }

  std::cout << "v1 value is " <<  v1->GetValue() << std::endl;
  std::cout << "v value is " <<  v->GetValue() << std::endl;
  //std::cout << " error unit " <<  (int)em->GetUnit() << std::endl;

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (1000.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (nPackets));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (1000.0));
// Changes made by Akilesh B
  //AnimationInterface anim ("anim1.xml");
  //anim.SetConstantPosition (nodes.Get(0), 1.0, 2.0);
  //anim.SetConstantPosition (nodes.Get(1), 2.0, 3.0);
// Changes made by Akilesh B
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
