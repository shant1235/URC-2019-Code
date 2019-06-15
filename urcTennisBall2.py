# import the necessary packages
from scipy.spatial import distance as dist
from picamera.array import PiRGBArray
from picamera import PiCamera
from imutils.video import VideoStream
import picamera
import numpy as np
import socket
import imutils
import time
import cv2
from fractions import Fraction


# SOCKET
UDP_IP = "192.168.137.235"	#can change
UDP_PORT = 8080              	#can change

sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP


# HSV COLOR VARIABLE
l_h = 23#33
l_s = 91#184
l_v = 145#75
h_h = 78#179
h_s = 255#255
h_v = 255#255

lower_yellow = np.array([l_h, l_s, l_v])
upper_yellow = np.array([h_h, h_s, h_v])
 
# SET UP VIDEO STREAM AND WAIT FOR CAMERA TO BE READY
vs = VideoStream(usePiCamera=1 > 0).start()
time.sleep(2.0)
 
 
# Initialize the camera and grab a reference to the raw camera capture
frameHeight = 480 #768
frameWidth = 640 #1024
centerFrame_W = frameWidth/2
centerFrame_H = frameHeight/2





#=========================================

# Background Subtraction 
#fgbg = cv2.createBackgroundSubtractorMOG()

# Let analog_gain and digital_gain settle then set exposure mode off



#=========================================
# Allow the camera to warmup
time.sleep(0.1)


# Capture frames from the camera
while True:
	
	frame = vs.read()
	
	# Background Subtraction 
	#fgmask = fgbg.apply(image)
	#cv2.imshow('fe')	
	
	# Blur
	blurred = cv2.GaussianBlur(frame, (11, 11), 0)				# Can change the kernel
	
	# HSV Thresholding
	hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)	
	mask = cv2.inRange(hsv, lower_yellow, upper_yellow)			# Can change HSV value
	
	# Erode
	eroded = cv2.erode(mask, None, iterations=2)				# Can change how much erosion
	
	# Dilate
	dilated = cv2.dilate(eroded, None, iterations=2)			# Can change how much dilation
	
	# Grayscale
	anded = cv2.bitwise_and(frame, frame, mask = dilated)
	gray = cv2.cvtColor(anded, cv2.COLOR_BGR2GRAY)	
	
	# Edge Dectection
	edges = cv2.Canny(gray, 50, 100)
	
	# Dilate Edge
	#dilated2 = cv2.dilate(edges, None, iterations=1)			# Can change how much dilation
	
	# Erode Edge
	#eroded2 = cv2.erode(dilated, None, iterations=1)			# Can change how much erosion
	
	#bffffffffff
	#bilateral_filtered_image = cv2.bilateralFilter(frame,5,175,175)
	#edge_detected_image = cv2.Canny(bilateral_filtered_image,75,200)
	
	# Grab Contours
	cnts = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
	cnts = imutils.grab_contours(cnts)

	data = "Not found"
	
	# Grab largest contour
	if (len(cnts) > 0):
		c = max(cnts, key=cv2.contourArea)
		#Drawing radius 
		((x,y),radius) = cv2.minEnclosingCircle(c)
		
		#fine tune the contour 
		epsilon = 0.25*cv2.arcLength(c,True)
		approx = cv2.approxPolyDP(c,epsilon,True)
		
		# Get center x and y
		M = cv2.moments(c)
		
		#center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
		
		if M["m00"] > 0:						# Make sure we don't divide by zero
			cX = int(M["m10"] / M["m00"])
			cY = int(M["m01"] / M["m00"])
	
			# Get area
			area = cv2.contourArea(c)
			k = cv2.isContourConvex(approx)
			if (area > 1):
				
				
				# Get box
				box = cv2.minAreaRect(c)	
				box = cv2.boxPoints(box)
				box = np.array(box, dtype = "int")
				box = box.astype("int")
				
				#Dh = dist.euclidean((box[0, 0], box[0, 1]), (box[1, 0], box[1, 1]))
				#Dw = dist.euclidean((box[1, 0], box[1, 1]), (box[2, 0], box[2, 1]))
				#if Dh != 0:
				#	aspect = Dw/Dh
					
				#	if abs(aspect - 1) <= 0.2: 	# We take all aspect ratio between 0.8 to 1.2 
						# Draw box and print center points
				cv2.drawContours(frame, [box], -1, (0, 255, 0), 1)
				cv2.putText(frame, "{:.0f},{:.0f}".format(cX - centerFrame_W, centerFrame_H - cY), (cX, cY), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (255, 255, 255), 1)
				
				#CIRCLE
				#cv2.circle(frame, (int(x) ,int(y)) ,int(radius), (0,255,255),2)
				#cv2.circle(frame, center, 5, (0,0,255),-1)
				
				
		
						# Send To
				data = str(int(cX - centerFrame_W)) + ", " + str(int(centerFrame_H - cY)) + ", " + str(int(area))
	try:
		sock.sendto(bytes(data,"utf-8"), (UDP_IP, UDP_PORT))
	except:
		pass
	print(data)
	#print(str(g) + "," + str(s))
	cv2.imshow("Frame", frame)						# Show frame
	cv2.imshow("BLur", blurred)
	cv2.imshow("hsv", hsv)
	cv2.imshow("MASK", mask)
	cv2.imshow("Masked", anded)
	cv2.imshow("Dilated", dilated)
	cv2.imshow("Eroded", eroded)
	cv2.imshow("Edge", edges)
	
	
	#cv2.imshow("Dilated", dilated2)
	#cv2.imshow("Eroded", eroded2)
	#cv2.imshow("Bilateral",bilateral_filtered_image)
	#cv2.imshow("Edge",edge_detected_image)


	key = cv2.waitKey(1) & 0xFF
 
	# if the `q` key was pressed, break from the loop
	if key == ord("q"):
		break