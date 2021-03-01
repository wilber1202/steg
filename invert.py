import sys
from PIL import Image, ImageChops

picname = sys.argv[1]
pic     = Image.open(picname)
newpic  = ImageChops.invert(pic)
newpicname = "/share/1_Steg/invert_" + picname
newpic.save(newpicname)

print("invert OK!")