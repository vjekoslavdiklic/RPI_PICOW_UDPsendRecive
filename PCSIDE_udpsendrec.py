import socket
import time

#configuration settings
Target_UDP_IP = "192.168.5.22"
Local_UDP_IP = socket.gethostbyname(socket.gethostname()) #Obtain automaticaly
UDP_PORT = 5005
BufferSize=1024
MESSAGE = b"B\n" #or b"B\n" for OFF
TimeOutInSec=0.5

#initialize
SockOut = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) # UDP socket for sending data
SockIn = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) # UDP socket for receiving data
SockIn.bind((Local_UDP_IP , UDP_PORT)) #bing to local ip and socket

# do in inf. loop send receive
while True:
    SockOut.sendto(MESSAGE, (Target_UDP_IP, UDP_PORT)) #send data
    print("UDP message has been send")
    data, addr = SockIn.recvfrom(BufferSize)  #recive data
    print("received message: %s" % data)
    time.sleep(TimeOutInSec) #wait for timeout
