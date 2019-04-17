"""
Fluidic Handling Software
Ashutosh Agarwal Lab
University of Miami

by:
Liev Birman
Adiel Hernandez
"""

import serial
#import serial
import sys
from serial.tools.list_ports import comports
import time


class TwoSwitch():
    """
    This class is used for control of the two switches. A serial connection is established with the microcontroller controlling the two switches.

    Attributes:
        baud (int): The baud rate the serial connection is using.
        port (string): the microcontrollers port for the serial connection.
        ser (serial Object): Instance of serial object representing the serial connection.
        serialConnected (bool): True/False of whether the serial connection was established.
        uniqueID (string): the unique identifier the microcontroller returns for automatic port selection.
        willRecirculate (bool): True/False as to if the two switches are set to recirculate.
    """

    def __init__(self,my_port):
        """
        The constructor for the TwoSwitch class.

        Parameters:
            my_port (string): the microcontrollers port for the serial connection.
        """

        self.baud = 9600
        self.port = my_port
        self.ser = None
        self.serialConnected = False
        self.uniqueID = ""

        #self.serial_connect()

        self.willRecirculate = False

    def serial_connect(self):
        """
        This method establishes the serial connection with the microcontroller.

        Once the comport of the pump is known we open a serial connection to it using pySerial.
        """

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
        This method sends a command across the serial connection.

        Parameters:
            cmd (string): The command or string that is to be sent to the microcontroller.
        """

        self.ser.write(cmd.encode('ascii') + '\r'.encode('ascii'))
        #self.ser.write(cmd.encode('ascii'))
        print("Sent a serial command: %s %s"%("TwoSwitch",cmd))
        #response = self.ser.readline().decode()
        #response = self.chop_return(response)
        #return response

    def chop_return(self,ret):
        """
        This method modifies the output of the two switch which comes with a carriage return and newline at the end of itself.
        The carriage return and newline is cut off here.

        Returns:
            Output of the pump without the carraige return and newline at the end.
        """

        if ret.endswith('\r\n'):
            ret = ret[:-2]
        return ret

    def setRecirculate(self, channel):
        """
        This method sends a command to the microcontroller based on what channel needs to be set to recirculate.

        Parameters:
            channel (int): The fluidic channel chosen to be set to recirculate.
        """

        self.willRecirculate = True
        if channel == 1:
            self.send('S')
        elif channel == 2:
            self.send('F')
        elif channel == 3:
            self.send('H')

    def setCollect(self, channel):
        """
        This method sends a command to the microcontroller based on what channel needs to be set to recirculate.

        Parameters:
            channel (int): The fluidic channel chosen to be set to collect.
        """

        self.willRecirculate = False
        if channel == 1:
            self.send('A')
        elif channel == 2:
            self.send('D')
        elif channel == 3:
            self.send('G')

    def getIdentifier(self):
        """
        This method sends the command "?" to the microcontroller which is programmed to send back a unique ID. The attribute uniqueID is set to the microcontrollers response.

        Returns:
            response (string): The unique ID that the microcontroller sends back through the serial connection.
        """

        self.send('?')
        self.ser.reset_input_buffer()
        response = self.ser.readline().decode()
        response = self.chop_return(response)
        self.uniqueID = response

        return self.uniqueID
