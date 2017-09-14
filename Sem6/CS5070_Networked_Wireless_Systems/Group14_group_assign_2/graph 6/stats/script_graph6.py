import os
folder='/Users/arjun/Downloads/grp_asn2_results/graph6/noop/100/'
for i in os.listdir(folder):
	if (i.startswith("no")):
		#print ' grep -i  \'Average throughput=\' '+ folder+i
		os.system(' grep -i  \'Average throughput=\' '+ folder+i)

