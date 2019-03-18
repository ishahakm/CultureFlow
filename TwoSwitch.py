import fakeSerial as serial
#import serial
import sys
from serial.tools.list_ports import comports
import time

"""


"""
class TwoSwitch():

    def __init__(self,my_port):
        self.baud = 9600
        self.port = my_port
        self.ser = None
        self.serialConnected = False
        self.uniqueID = ""

        self.serial_connect()

        self.willRecirculate = False

    # def connect(self):
    #     """runs get_port and then serial_connect. They have to be run
    #     in series"""
    #     port_found = self.get_port()
    #     if port_found == True:
    #         self.serial_connect()
    #     else:
    #         print(" no port found")

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
        uses serial connection opened instance of TwoSwitch and sends
        the text written in cmd across that connection.
        Function had option
        """
        self.ser.write(cmd.encode('ascii') + '\r'.encode('ascii'))
        #self.ser.write(cmd.encode('ascii'))
        print("Sent a serial command: %s %s"%("TwoSwitch",cmd))
        #response = self.ser.readline().decode()
        #response = self.chop_return(response)
        #return response

    def chop_return(self,ret):
        """output of the switch comes with a carriage return and newline at the end of itself.
        We chop them off here.
        """
        if ret.endswith('\r\n'):
            ret = ret[:-2]
        return ret

    def setRecirculate(self, channel):
        """sets the TwoSwitch to recirulate the fluid to the chip, will send serial
        command to the microcontroller depending on which switch instance called the method.
        """
        self.willRecirculate = True
        if channel == 1:
            self.send('S')
        elif channel == 2:
            self.send('F')
        elif channel == 3:
            self.send('H')

    def setCollect(self, channel):
        """sets the TwoSwitch to collect the fluid on the well plate, will send serial
        command to the microcontroller depending on which switch instance called the method.
        """
        self.willRecirculate = False
        if channel == 1:
            self.send('A')
        elif channel == 2:
            self.send('D')
        elif channel == 3:
            self.send('G')

    def getIdentifier(self):
        """sends a serial command to the Arduino asking it to send its unique identifier.
        This identifier is saved under the variable uniqueID.
        """
        self.send('?')
        self.ser.reset_input_buffer()
        response = self.ser.readline().decode()
        response = self.chop_return(response)
        self.uniqueID = response

        return self.uniqueID
