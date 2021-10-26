#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
import subprocess
import os
			 
if (1 == len(sys.argv)):
	print("Miss file...")
	sys.exit(0)
	
filename	= sys.argv[1]

steg_tools	= [("", "strings -a", "> result_strings.txt"), 
		   	   ("", "remove0x00", ""),
		   	   ("", "zsteg -a", "> result_zsteg.txt"), 
		   	   ("", "file_type", "> result_filetype.txt"),
		   	   ("png", "pngcheck -vf", "> result_pngcheck.txt"),
		   	   ("", "outguess -r", "result_outguess.txt"),
		   	   ("gif", "identify -format \"%T\"", "> result_identify.txt"),
		   	   ("", "exiftool", "> result_exiftool.txt"),
		   	   ("", "file_modify", ""),
		   	   ("", "python /usr/bin/lsb.py extract", "result_lsb.txt 123456"),
		   	   ("", "stepic -d -i", "> result_stepic.txt"),
			   ("", "python /usr/bin/invert.py", ""),
		   	   ("gif", "convert", "frame.png"),
		   	   ("gif", "python /usr/bin/invert_frame.py", ""),
		   	   ("", "steghide extract -p 123456 -sf", ""),
		   	   ("png", "python /usr/bin/apng_split.py", ""),
		   	   ("", "foremost", ""),
		   	   ("", "binwalk -Me", ""),
		   	   ("", "what_steg -f", ""),
		   	   ("", "python /usr/bin/recursion.py", "")]
		   	   #("png", "python3 /usr/bin/brute_png_height.py", "")]

for i in range(0, len(steg_tools)):
	if ((0 != len(steg_tools[i][0])) and (False == filename.endswith(steg_tools[i][0]))):
		continue
	cmd = steg_tools[i][1] + " " + filename + " " + steg_tools[i][2]
	print(cmd)
	result = subprocess.Popen(cmd, shell = True, stdout = subprocess.PIPE, stderr = subprocess.STDOUT)
	result.wait()
	lines = result.stdout.readlines()
	for line in lines:
		print("%s" % line)

#cmd = "python /usr/bin/invert.py right_height.png"
#subprocess.Popen(cmd, shell = True, stdout = subprocess.PIPE, stderr = subprocess.STDOUT)

#cmd = "python /usr/bin/lsb.py extract right_height.png result_right_height_lsb.txt 123456"
#subprocess.Popen(cmd, shell = True, stdout = subprocess.PIPE, stderr = subprocess.STDOUT)
