import os
import sys
algorithms = ['ns3::NoOpHandoverAlgorithm'];
mapping = {'ns3::NoOpHandoverAlgorithm': 'noOp'};
speed =[20,50,100];
#ipi = [1,10];
#map2={'1':'f','10':'h'};
rngrun=[42,43,44,45,46];

for a in algorithms:
	for vel in speed:
			for rng in rngrun:
				outfile='graph6/'+mapping[a]+'_'+str(vel)+'_'+str(rng)
				print './waf --run \"scratch/lena-noop-handover --speed='+str(vel)+' --handAlgo='+a+' --RngRun='+str(rng)+'\"'+' >'+ outfile+'.txt'
				os.system('./waf --run \"scratch/lena-noop-handover --speed='+str(vel)+' --handAlgo='+a+' --RngRun='+str(rng)+'\"'+' >'+ outfile+'.txt')
				#os.system('cp DlRsrpSinrStats.txt graph2/DlRsrpSinrStats_'+mapping[a]+'_'+str(vel)+'_'+str(rng)+'.txt')
				#os.system('cp DlPdcpStats.txt graph2/DlPdcpStats_'+mapping[a]+'_'+str(vel)+'_'+str(rng)+'.txt')


