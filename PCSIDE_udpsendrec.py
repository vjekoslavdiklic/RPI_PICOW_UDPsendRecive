import socket,time
import tkinter as tk
from tkinter import ttk
import numpy as np
#configuration settings
Target_UDP_IP = "192.168.5.22"
Local_UDP_IP = socket.gethostbyname(socket.gethostname()) #Obtain automaticaly
UDP_PORT = 5005
BufferSize=1024
MESSAGE = b"B\n" #or b"B\n" for OFF
TimeOutInSec=0.5

def LedON():
    global MESSAGE
    MESSAGE = b"A\n"

def LedOFF():
    global MESSAGE
    MESSAGE = b"B\n"

#initialize
SockOut = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) # UDP socket for sending data
SockIn = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) # UDP socket for receiving data
SockIn.bind((Local_UDP_IP , UDP_PORT)) #bing to local ip and socket

#windows settings:
window = tk.Tk()
window.geometry('300x120')
window.title('UDP_Readout&Control')
window.grid()

#progress bar settings:
pb = ttk.Progressbar(window,orient='horizontal',mode='determinate',length=280)
pb.grid(column=0, row=0, columnspan=2, padx=10, pady=20)
start_button = ttk.Button(window,text='Turn ON',command=LedON)
start_button.grid(column=0, row=1, padx=10, pady=10, sticky=tk.E)
stop_button = ttk.Button(window,text='Turn OFF',command=LedOFF)
stop_button.grid(column=1, row=1, padx=10, pady=10, sticky=tk.W)

#UDP communication function:
def updateudpdata():
    global MESSAGE
    SockOut.sendto(MESSAGE, (Target_UDP_IP, UDP_PORT)) #send data
    print("UDP message has been send")
    data, addr = SockIn.recvfrom(BufferSize)  #recive data
    print("received message: %s" % data)
    a = str(data)
    b = a.split('A=')
    c = b[1]
    c = c[0:-1]
    c = int(c)
    d = (c / 1023) * 100
    d = int(d)
    pb['value'] = d

#main loop
while True:
    window.after(0, updateudpdata)
    window.update()
window.mainloop()
