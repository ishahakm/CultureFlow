import fakeSerial as serial
#import serial
import sys
from serial.tools.list_ports import comports
import time

class Collectador():

    def __init__(self,my_port):
        self.baud = 9600
        self.port = my_port
        self.ser = None
        self.serialConnected = False
        self.res = None
        self.position = None
        self.uniqueID = ""

        self.serial_connect()

        #The sleep for a second is needed for serial connection to be connected so reset could be run.
        time.sleep(1)
        self.reset()

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
        #self.ser.write(cmd.encode('ascii'))
        print("Sent a serial command: %s %s"%("Collectador",cmd))
    def chop_return(self,ret):
        """output of the pump comes with a carriage return and newline at the end of itself.
        We chop them off here.
        """
        if ret.endswith('\r\n'):
            ret = ret[:-2]
        return ret
    def reset(self):
        self.send('Z')
        self.position = 0
        #print(self.position)
    def eject(self):
        self.send('E')
        self.position = 32
        #print(self.position)
    def next_site(self):
        if self.position < 31:
            self.send('N')
            self.position += 1
            #print(self.position)
    def last_site(self):
        if self.position > 0:
            self.send("L")
            self.position -= 1
            #print(self.position)
    def get_info(self):
        self.ser.reset_input_buffer()
        self.send('?')
        response = self.ser.readline().decode()
        response = self.chop_return(response)
        self.uniqueID = response

        return response
