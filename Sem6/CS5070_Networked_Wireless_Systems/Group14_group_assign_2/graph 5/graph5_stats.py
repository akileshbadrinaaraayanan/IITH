import os
import sys
algorithms = ['ns3::A2A4RsrqHandoverAlgorithm'];
mapping = {'ns3::A2A4RsrqHandoverAlgorithm': 'first'};
speed =[20,50,100];
#ipi = [1,10];
#map2={'1':'f','10':'h'};
celloff=[1,2,5];
rngrun=[42,43,44,45,46];

for a in algorithms:
	for vel in speed:
		for h in celloff:
			for rng in rngrun:
				outfile='graph5/'+mapping[a]+'_'+str(h)+'_'+str(vel)+'_'+str(rng)
				print './waf --run \"scratch/lena-cell-offset --speed='+str(vel)+' --handAlgo='+a +' --celloffset='+str(h)+' --RngRun='+str(rng)+'\"'+' >'+ outfile+'.txt'
				os.system('./waf --run \"scratch/lena-cell-offset --speed='+str(vel)+' --handAlgo='+a +' --celloffset='+str(h)+' --RngRun='+str(rng)+'\"'+' >'+ outfile+'.txt')
				#os.system('cp DlRsrpSinrStats.txt graph2/DlRsrpSinrStats_'+str(h)+'_'+mapping[a]+'_'+str(vel)+'_'+str(rng)+'.txt')
				#os.system('cp DlPdcpStats.txt graph2/DlPdcpStats_'+str(h)+'_'mapping[a]+'_'+str(vel)+'_'+str(rng)+'.txt')


