import sys
import re

outlogfile = open(sys.argv[2],'w')
inlogfile = open(sys.argv[1])

for line in inlogfile:

	date = re.search('[0-9][0-9]/[0-9][0-9]/[0-9][0-9]',line)
	url = re.search('http://.*? ',line)
	time = re.search('[0-9][0-9]:[0-9][0-9]:[0-9][0-9].[0-9][0-9]',line)

	date_modified = date.group()[3:5]+"/"+date.group()[0:2]+"/"+date.group()[6:8]

	output = "%s\t%s\t%s\n" %(url.group(),date_modified,time.group())
	outlogfile.write(output)

outlogfile.close()
inlogfile.close()