import socket,time
import tkinter as tk
from tkinter import ttk
import numpy as np
#configuration settings
Target_UDP_IP = "192.168.5.22"
Local_UDP_IP = socket.gethostbyname(socket.gethostname()) #Obtain automaticaly
UDP_PORT = 5005
BufferSize=1024
MESSAGE = b"B\n" #or b"B\n" for OFF LEDCommand_MODE_ul_LL_HL
Mode=0
LimUpdate=0
HL=900
LL=100
LEDSTATUS=0
#examples:
#1_1_0 turn on led, mode 1, dont set limits
#0_0_1_200_900 turn off led, mode 0, update limits ,Ll=200, HL=900
TimeOutInSec=0.5

def LedON():
    global LEDSTATUS
    LEDSTATUS = 1
    DefineAMessage()

def LedOFF():
    global LEDSTATUS
    LEDSTATUS = 0
    DefineAMessage()

def mode0():
    global Mode
    Mode=0
    DefineAMessage()

def mode1():
    global Mode
    Mode=1
    DefineAMessage()

def DefineAMessage():
    global LEDSTATUS
    global HL
    global LL
    global LimUpdate
    global MESSAGE
    global Mode
    MESSAGE = str.encode(str(LEDSTATUS)+"_" + str(Mode) + "_" + str(LimUpdate) + "_" + str(LL) + "_" + str(HL))

def setHL():
    global HL
    HL=HLB.get()
    DefineAMessage()

def setLL():
    global LL
    LL=LLB.get()
    DefineAMessage()

#initialize
SockOut = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) # UDP socket for sending data
SockIn = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) # UDP socket for receiving data
SockIn.bind((Local_UDP_IP , UDP_PORT)) #bing to local ip and socket

#windows settings:
window = tk.Tk()
window.geometry('280x240')
window.title('UDP_Readout&Control')
window.grid()

#progress bar settings:
pb = ttk.Progressbar(window,orient='horizontal',mode='determinate',length=165)
pb.grid(column=0, row=3, columnspan=5, padx=20, pady=20)
start_button = ttk.Button(window,text='Turn ON',command=LedON)
start_button.grid(column=0, row=1, padx=10, pady=10, sticky=tk.E)
stop_button = ttk.Button(window,text='Turn OFF',command=LedOFF)
stop_button.grid(column=1, row=1, padx=10, pady=10, sticky=tk.W)

m0_button = ttk.Button(window,text='Mode 0',command=mode0)
m0_button.grid(column=0, row=2, padx=10, pady=10, sticky=tk.W)

m1_button = ttk.Button(window,text='Mode 1',command=mode1)
m1_button.grid(column=1, row=2, padx=10, pady=10, sticky=tk.W)

HLB=ttk.Entry(window,textvariable =HL,width=5)
HLB.insert(0,str(HL))
HLB.grid(column=0, row=4, padx=10, pady=10, sticky=tk.W)

HLBb=ttk.Button(window,text="SET High Level",command=setHL)
HLBb.grid(column=1, row=4, padx=10, pady=10, sticky=tk.W)

LLB=ttk.Entry(window,textvariable =LL,width=5)
LLB.insert(0,str(LL))
LLB.grid(column=0, row=5, padx=10, pady=10, sticky=tk.W)

LLBb=ttk.Button(window,text="SET Low Level",command=setLL)
LLBb.grid(column=1, row=5, padx=10, pady=10, sticky=tk.W)

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
    c=c.split('_')

    c = int(c[0])
    d = (c / 1023) * 100
    d = int(d)
    pb['value'] = d

#main loop
while True:
    window.after(0, updateudpdata)
    window.update()
window.mainloop()
