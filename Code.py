import pygame
import socket
import time
from pygame.locals import *

pygame.joystick.init()
pygame.init()
joysticks = [pygame.joystick.Joystick(x) for x in range(pygame.joystick.get_count())]
joysticks[0].init()
UDP_IP = "192.168.1.100"
UDP_PORT = 8080

print("The number of joysticks: " + str(pygame.joystick.get_count()))
print("The name of joystick: " + joysticks[0].get_name())
print("The number of axis: " + str(joysticks[0].get_numaxes()))

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

X = 50;
while keepRunning:
    axisY = 128 + round(joysticks[0].get_axis(0), 1) * -X + round(joysticks[0].get_axis(1), 1) * -X
    axisX = 128 + round(joysticks[0].get_axis(1), 1) * -X - round(joysticks[0].get_axis(0), 1) * -X

    for event in pygame.event.get():
            #time.sleep(1)
            #axisX = round((joysticks[0].get_axis(0) + 1) * (-255 / 2)) + 255 (Old format dont use)
            #axisY = round((joysticks[0].get_axis(1) + 1) * (-255 / 2)) + 255 (Old format dont use)
            axisY = 128 + round(joysticks[0].get_axis(0),1) * X + round(joysticks[0].get_axis(1),1) * X
            axisX = 128 + round(joysticks[0].get_axis(1),1) * X - round(joysticks[0].get_axis(0),1) * X
            clock.tick(10
                       )
            print("LeftM: " + str(axisX))
            print("RightM: " + str(axisY) + "\n\n")
            # print("PositionX: " + str(round(joysticks[0].get_axis(0),1))) (Old format dont use)
            # print("PositionY: " + str(round(joysticks[0].get_axis(1),1)) + "\n\n") (Old format dont use)
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
