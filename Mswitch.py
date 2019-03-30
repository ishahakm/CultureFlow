#import serial
import serial
import sys
from serial.tools.list_ports import comports
import time

class MSwitch():
    """
    This class is used for control of the MSwitch. A serial connection is established
    with the microcontroller controlling the MSwitch.

    Attributes:
        baud (int): The baud rate the serial connection is using.
        port (string): the microcontrollers port for the serial connection.
        ser (serial Object): Instance of serial object representing the serial connection.
        serialConnected (bool): True/False of whether the serial connection was established.
        res (int): The reservoir chosen.
        uniqueID (string): the unique identifier the microcontroller returns for automatic port selection.

    """

    def __init__(self,my_port):
        """
        The constructor for the MSwitch class.

        Parameters:
            my_port (string): the microcontrollers port for the serial connection.
        """

        self.baud = 9600
        self.port = my_port
        self.ser = None
        self.serialConnected = False
        self.res = None
        self.uniqueID = None

        self.serial_connect()

    def serial_connect(self):
        """
        This method establishes the serial connection with the microcontroller.

        Once the comport of the pump is known we open a serial connection
        to it using pySerial.
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
        print("Sent a serial command: %s %s "%("Mswitch",cmd))
        #response = self.ser.readline().decode()
        #response = self.chop_return(response)
        #return response
    def chop_return(self,ret):
        """
        This method modifies the output of the pump which comes with a carriage return and newline at the end of itself.
        The carriage return and newline is cut off here.

        Returns:
            Output of the pump without the carraige return and newline at the end.
        """

        if ret.endswith('\r\n'):
            ret = ret[:-2]
        return ret

    def set_reservoir(self,res):
        """
        This method sets the reservoir to pull fluid from.
        """
        self.send("P0"+str(res))

    def get_info(self):
        """
        This method sends the command "?" to the microcontroller which is programmed to
        send back a unique ID. The attribute uniqueID is set to the microcontrollers response.

        Returns:
            response (string): The unique ID that the microcontroller sends back through the serial connection.
        """

        self.send("S")
        response = self.ser.readline().decode()
        response = self.chop_return(response)
        self.uniqueID = response
        return response
