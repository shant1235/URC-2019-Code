import socket

UDP_IP = "192.168.1.210"
UDP_PORT = 8090

UDP_IP222 = "192.168.1.132"
UDP_PORT222 = 8090

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock2 = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

sock.sendto(bytes("PINGpingPing", "utf-8"), (UDP_IP, UDP_PORT))
sock2.bind((UDP_IP222, UDP_PORT222))
file = open("RoverTest8.txt", "w")

while True:
    file = open("RoverTest8.txt", "a")
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print (data.decode("utf-8"))
    file.write(data.decode("utf-8") + "\n")
    file.close()