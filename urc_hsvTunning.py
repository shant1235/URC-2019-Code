from imutils.video import VideoStream
import numpy as np
import imutils
import time
import cv2

import picamera
import socket 

# BUILD TRACKER WINDOW AND SLIDERS
def nothing(x):
	pass
	
cv2.namedWindow("Trackbars")	#create name for the window
cv2.createTrackbar("L - H", "Trackbars", 0, 179, nothing)
cv2.createTrackbar("L - S", "Trackbars", 0, 255, nothing)
cv2.createTrackbar("L - V", "Trackbars", 0, 255, nothing)
cv2.createTrackbar("U - H", "Trackbars", 179, 179, nothing)
cv2.createTrackbar("U - S", "Trackbars", 255, 255, nothing)
cv2.createTrackbar("U - V", "Trackbars", 255, 255, nothing)


# SET UP VIDEO STREAM AND WAIT FOR CAMERA TO BE READY
vs = VideoStream(usePiCamera=1 > 0).start()
time.sleep(2.0)


#Grab Frame from Queue
while True:
	frame = vs.read()
	#frame = imutils.resize(frame, width=1920)	# Resize
	
	# Blur
	blurred = cv2.GaussianBlur(frame, (11, 11), 0)	
	# Hsv
	hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)
	
	l_h = cv2.getTrackbarPos("L - H","Trackbars")
	l_s = cv2.getTrackbarPos("L - S","Trackbars")
	l_v = cv2.getTrackbarPos("L - V","Trackbars")
	u_h = cv2.getTrackbarPos("U - H","Trackbars")
	u_s = cv2.getTrackbarPos("U - S","Trackbars")
	u_v = cv2.getTrackbarPos("U - V","Trackbars")
	
	lower_hsv = np.array([l_h, l_s, l_v])
	upper_hsv = np.array([u_h, u_s, u_v])
	
	mask = cv2.inRange(hsv, lower_hsv, upper_hsv)
	
	#Erode
	eroded = cv2.erode(mask, None, iterations=2)				# Can change how much erosion
	
	#Dilate
	dilated = cv2.dilate(eroded, None, iterations=2)			# Can change how much dilation
	
	frame = cv2.bitwise_and(frame, frame, mask = dilated)
	
	cv2.imshow("Frame", frame)			# Show frame
	cv2.imshow("mask", mask)
	cv2.imshow("eroded", eroded)		
	cv2.imshow("dilated", dilated)	
	
	key = cv2.waitKey(1)				# Exit condition, pres "q"
	if key == ord('q'):
		break
		
cv2.destroyAllWindows()					# Clean up
vs.stop()
