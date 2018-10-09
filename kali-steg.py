import sys
import subprocess
import os
			 
if (1==len(sys.argv)):
	print("Miss file...")
	sys.exit(0)
	
filename=sys.argv[1]

steg_tools=[("", "strings -a", "> result_strings.txt"), 
		   			("", "zsteg", "> result_zsteg.txt"), 
		   			("", "file_type", "> result_filetype.txt"),
		   			("png", "pngcheck -vf", "> result_pngcheck.txt"),
		   			("", "outguess -r", "result_outguess.txt"),
		   			("", "foremost", ""),
		   			("gif", "identify -format \"%T\"", "> result_identify.txt"),
		   			("", "exiftool", "> result_exiftool.txt"),
		   			("", "binwalk -e", ""),
		   			("", "file_modify", ""),
		   			("", "python /usr/bin/lsb.py extract", "result_lsb.txt 123456")]
	
for i in range(0, len(steg_tools)):
	if ((0 != len(steg_tools[i][0])) and (False == filename.endswith(steg_tools[i][0]))):
		continue
	cmd = steg_tools[i][1] + " " + filename + " " + steg_tools[i][2]
	print(cmd)
	result = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
	result.wait()
	lines = result.stdout.readlines()
	for line in lines:
		print("%s" % line)