import Descriptor
import math
import numpy as np
import imageio

pathimgs = "../input/corel1k/data.csv"

f=open(pathimgs)
lines=f.readlines()
for i in range(5, len(lines)):
    line = lines[i].split(",")
    pathimg = "../input/corel1k/"+line[0]
    img = imageio.imread(pathimg)
    
    dx = [0,0,0,1,3,5]
    dy = [1,3,5,0,0,0]
    fe = Descriptor.GLCooMatrix(dx, dy, img)
    print (fe)
        
    fe = Descriptor.BIC(img)
    print (fe)
    break
    