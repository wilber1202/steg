# -*- coding: utf-8 -*-
import os
from PIL import Image, ImageChops

for i in range(500):
    picname = "/share/1_Steg/frame-" + str(i) + ".png"
    if (False == os.access(picname, os.F_OK)):
        continue
    pic     = Image.open(picname)
    newpic  = ImageChops.invert(pic)
    newpicname = "/share/1_Steg/newframe-" + str(i) + ".png"
    newpic.save(newpicname)

print("invert frame OK!")
