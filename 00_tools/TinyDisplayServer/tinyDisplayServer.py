# -*- coding: utf-8 -*-
import sys
import os
import struct
from PIL import Image
import serial
import pyautogui

OUT_SIZE = 128, 80

###
# Main
###
def main():
	while 1:
		s = pyautogui.screenshot()
		s.save('temp.png')
		sendDataUART('temp.png')




###
# sendDataUART
# brief: load image file, convert into RGB565, transfer via UART
###
def sendDataUART(fileName):
	image = Image.open(fileName)
	image.thumbnail(OUT_SIZE,Image.ANTIALIAS)
	#image.save("temp.png")

	width = image.size[0]
	height = image.size[1]
	startX = (OUT_SIZE[0] - width) / 2
	startY = (OUT_SIZE[1] - height) / 2
	endX = startX + width
	endY = startY + height
	

	#ser = serial.Serial("COM3", 9600, stopbits = serial.STOPBITS_TWO)
	#ser.setDTR(False)
	ser = serial.Serial()
	ser.port = "COM3"
	ser.baudrate = 2000000
	ser.timeout = 1
	ser.stopbits = serial.STOPBITS_TWO
	ser.setDTR(False)
	ser.open()
	for y in range(OUT_SIZE[1]):
		for x in range(OUT_SIZE[0]):
			if( (x < startX) or (x >= endX) or (y < startY) or (y >= endY) ):
				data = 0
			else:
				data = convertRGB2RGB565(image.getpixel((x - startX, y - startY)))
			dataHigh = data >> 8
			dataLow = data & 0xFF
			odata = struct.pack('BB', dataHigh, dataLow)
			#odata = struct.pack('BB', dataLow, dataHigh)
			ser.write(odata)
	ser.close()

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

