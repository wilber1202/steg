# -*- coding: utf-8 -*-
import sys
import subprocess
import os
	
filename 	= "Misc.jpg"
path 		= "E:\\7 Share\\1_steg\\"

steg_tools  = [("jpg", "\"E:\\6 Rolan\\Tools\\009\\stegdetect\\stegdetect.exe\" -tjopi -s 10.0", ""),
			   ("jpg", "\"E:\\6 Rolan\\Tools\\009\\stegdetect\\stegbreak.exe\" -r \"E:\\6 Rolan\\Tools\\009\\stegdetect\\rules.ini\" -f \"E:\\6 Rolan\\Tools\\009\\stegdetect\\wordlist.txt\" -t p", ""),
			   ("", "\"E:\\6 Rolan\\Tools\\010\\bftools\\bftools.exe\" decode braincopter", ""),
			   ("", "\"E:\\6 Rolan\\Tools\\010\\bftools\\bftools.exe\" decode brainloller", ""),
			   ("jpg", "java -classpath \"E:\\6 Rolan\\Tools\\009\\F5-steganography\" Extract", ""),
			   ("jpg", "java -classpath \"E:\\6 Rolan\\Tools\\009\\F5-steganography\" Extract -p 123456 -e output1.txt", "")]

for i in range(0, len(steg_tools)):
	if ((0 != len(steg_tools[i][0])) and (False == filename.strip('"').lower().endswith(steg_tools[i][0]))):
		continue
	cmd = steg_tools[i][1] + " " + "\"" + path + filename + "\"" + " " + steg_tools[i][2]
	print(cmd)
	result = subprocess.Popen(cmd, shell = True, stdout = subprocess.PIPE, stderr = subprocess.STDOUT)
	result.wait()
	lines = result.stdout.readlines()
	for line in lines:
		print("%s" % line)
	print("*******************************************************************************")
