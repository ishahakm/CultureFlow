#import serial
import serial
import sys
from serial.tools.list_ports import comports
import time

class MSwitch():

    def __init__(self,my_port):
        self.baud = 9600
        self.port = my_port
        self.ser = None
        self.serialConnected = False
        self.res = None

        self.serial_connect()
    def serial_connect(self):
        """Once the comport of the pump is known we open a serial connection
        to it using pySerial"""

        if self.port == None:
            self.connected = False
        else:
            self.ser = serial.Serial(self.port, self.baud, timeout=3)
            if self.ser.isOpen():
                self.connected = True
            else:
                self.connected = False
    def send(self,cmd):
        """
        uses serial connection opened instance of pump and sends
        the text written in cmd across that connection.
        Function had option
        """
        self.ser.write(cmd.encode('ascii') + '\r'.encode('ascii'))
        print("Sent a serial command: %s %s "%("Mswitch",cmd))
        #response = self.ser.readline().decode()
        #response = self.chop_return(response)
        #return response
    def chop_return(self,ret):
        """output of the pump comes with a carriage return and newline at the end of itself.
        We chop them off here.
        """
        if ret.endswith('\r\n'):
            ret = ret[:-2]
        return ret
    def set_reservoir(self,res):
        self.send("P0"+str(res))
    def get_info(self):
        self.send("S")
        response = self.ser.readline().decode()
        response = self.chop_return(response)
        return response
