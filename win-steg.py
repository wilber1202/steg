import sys
import subprocess
import os
			 
if (1==len(sys.argv)):
	print("Miss file...")
	sys.exit(0)
	
filename=sys.argv[1]
path="D:\Share\\1steg\\"

steg_tools=[("zip", "java -jar D:\Rolan\Tools\\004\zip\ZipCenOp.jar r", ""),
						("", "D:\Rolan\Tools\\010\\bftools\\bftools.exe decode braincopter", ""),
						("", "D:\Rolan\Tools\\010\\bftools\\bftools.exe decode brainloller", "")]
	
for i in range(0, len(steg_tools)):
	if ((0 != len(steg_tools[i][0])) and (False == filename.endswith(steg_tools[i][0]))):
		continue
	cmd = steg_tools[i][1] + " " + path + filename + " " + steg_tools[i][2]
	print(cmd)
	result = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
	result.wait()
	lines = result.stdout.readlines()
	for line in lines:
		print("%s" % line)