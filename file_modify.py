# -*- coding: utf-8 -*-
"""
Created on Fri Sep  7 15:20:38 2018

@author: wilber
"""

import sys

#def file_type(data):
#    if (data)

file = open("02.jpg", 'rb')
data = file.read()

print "%x" % data[0]
print(data[1])
print(data[2])
print(data[3])

if (0xff == data[0]):
    print(1)
else:
    print(2)

#new_file_name = 'new_' + sys.argv[1]
#print(new_file_name)
