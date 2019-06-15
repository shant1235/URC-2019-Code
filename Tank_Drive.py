import pygame
import socket
import time

pygame.joystick.init()
pygame.init()
joysticks = [pygame.joystick.Joystick(x) for x in range(pygame.joystick.get_count())]
joysticks[0].init()

UDP_IP = "192.168.1.100"
UDP_PORT = 8080

print("The number of joysticks: " + str(pygame.joystick.get_count()))
print("The name of joystick: " + joysticks[0].get_name())
print("The number of axis: " + str(joysticks[0].get_numaxes()))
# Note: Axis 0,1 for left trigger, Axis 2,3 for right trigger
# axisX and axisY can also be called LeftMotors and RightMotors
keepRunning = True
clock = pygame.time.Clock()


def numToStringF(num):
    # Make each pwm value 3 characters
    num = int(num)
    ans = str(num)
    if len(ans) == 1:
        return "00" + ans
    elif len(ans) == 2:
        return "0" + ans
    else:
        return ans
axisX = 0;
axisY = 0;
X = 50;
while keepRunning:
    axisY = 128 + round(joysticks[0].get_axis(3), 1) * X
    axisX = 128 + round(joysticks[0].get_axis(1), 1) * X
    if (joysticks[0].get_button(7) == True)& (X < 120):
        X = X + 5
        time.sleep(0.5)
        print("UP SPEED: " + str(X))
    elif (joysticks[0].get_button(6) == True) & (X > 5):
        X = X - 5
        time.sleep(0.5)
        print("DOWN SPEED: " + str(X))
    for event in pygame.event.get():
            #time.sleep(1)
            axisY = 128 + round(joysticks[0].get_axis(3), 1) * X
            axisX = 128 + round(joysticks[0].get_axis(1), 1) * X
            if axisY < 0:
                axisY = 0
            elif axisY > 255:
                axisY = 255
            if axisX < 0:
                axisX = 0
            elif axisX > 255:
                axisX = 255
            clock.tick(10)
            print("LeftM: " + str(axisX))
            print("RightM: " + str(axisY) + "\n\n")

            pygame.event.pump()

            MESSAGE = "d" + numToStringF(axisX) + numToStringF(axisX) + numToStringF(axisX) + numToStringF(
                axisY) + numToStringF(axisY) + numToStringF(axisY)

            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP
            if axisX >= 132 or axisX <= 124 or axisY >= 132 or axisY <= 124:
                sock.sendto(bytes(MESSAGE, "utf-8"), (UDP_IP, UDP_PORT))
                print(MESSAGE)
            else:
                MESSAGE = "d128128128128128128"
                sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP
                sock.sendto(bytes(MESSAGE, "utf-8"), (UDP_IP, UDP_PORT))
                print(MESSAGE)
