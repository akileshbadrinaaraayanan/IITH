import os
folder='/home/akilesh/Downloads/ns-allinone-3.24/ns-3.24/results/fdbetff/h/'
for i in os.listdir(folder):
	if (i.startswith("stat3_")==0):
		os.system(' grep -i  \'throughput\' '+ folder+i+ ' >' + folder+'stat3_'+i)
