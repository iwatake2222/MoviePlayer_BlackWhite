# -*- coding: utf-8 -*-
import sys
import os
import struct
from PIL import Image

OUT_SIZE = 128, 80

###
# Main
###
def main():
	if( len(sys.argv) != 2 ):
		print 'Usage: \n# python %s filename' % sys.argv[0]
		quit()
	inputFilename = sys.argv[1]
	print '... processing %s' % inputFilename
	outputFilename = os.path.splitext(inputFilename)[0] + ".bin"

	image = Image.open(inputFilename)
	image.thumbnail(OUT_SIZE,Image.ANTIALIAS)
	#image.save("temp.png")

	width = image.size[0]
	height = image.size[1]
	startX = (OUT_SIZE[0] - width) / 2
	startY = (OUT_SIZE[1] - height) / 2
	endX = startX + width
	endY = startY + height
	
	outFile = open(outputFilename, "wb")
	odata = ''
	for y in range(OUT_SIZE[1]):
		for x in range(OUT_SIZE[0]):
			if( (x < startX) or (x >= endX) or (y < startY) or (y >= endY) ):
				data = 0
			else:
				data = convertRGB2RGB565(image.getpixel((x - startX, y - startY)))
			dataHigh = data >> 8
			dataLow = data & 0xFF
			odata += struct.pack('BB', dataHigh, dataLow)
	outFile.write(odata)
	outFile.close()


###
# convertRGB2RGB565
# brief: create RGB565 data
###
def convertRGB2RGB565(rgba):
	r = rgba[0] >> 3
	g = rgba[1] >> 2
	b = rgba[2] >> 3
	dataHigh = r << 3 | g >> 3
	dataLow = (g & 0x07) << 5 | b
	return dataHigh<< 8 | dataLow



###
# Main
###
if __name__ == "__main__":
	main()

