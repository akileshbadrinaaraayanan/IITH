import cv2
import numpy as np
import os
list = []

for file in os.listdir("/home/akilesh/Downloads/lfwcrop_color/faces"):
    if file.endswith(".ppm"):
       # print(file)
        list.append(file)

#for i in list:
#        print i

for i in list:
	print i
	img = cv2.imread('faces/'+i)

	height, width = img.shape[:2]
	#print height
	#print width
	res = cv2.resize(img,(32, 32), interpolation = cv2.INTER_AREA)
	cv2.imwrite('faces32/'+i, res)
