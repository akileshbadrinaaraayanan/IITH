import os
import sys
schedulers = ["ns3::PfFfMacScheduler","ns3::RrFfMacScheduler","ns3::FdMtFfMacScheduler","ns3::FdBetFfMacScheduler"];
mapping = {'ns3::PfFfMacScheduler': 'pfff', 'ns3::RrFfMacScheduler': 'rrff', 'ns3::FdMtFfMacScheduler': 'fdmtff','ns3::FdBetFfMacScheduler':'fdbetff'};
speed =[0,5];
ipi = [1,10];
map2={'1':'f','10':'h'};
rngrun=[42,43,44,45,46];

for s in schedulers:
	for vel in speed:
		for buf in ipi:
			for rng in rngrun:
				outfile='testoutput/'+mapping[s]+'_'+map2[str(buf)]+'_'+str(vel)+'_'+str(rng)
				#print './waf --run \"ass1_cmd --speed='+str(vel)+ ' --interPacketInterval='+str(buf)+' --schedType='+s+'--RngRun='+str(rng)+'\"'+' >'+ outfile+'.txt'
				os.system('./waf --run \"ass1_cmd --speed='+str(vel)+ ' --interPacketInterval='+str(buf)+' --schedType='+s+' --RngRun='+str(rng)+'\"'+' >'+ outfile+'.txt')
