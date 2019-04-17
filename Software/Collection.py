"""
Fluidic Handling Software
Ashutosh Agarwal Lab
University of Miami

by:
Liev Birman
Adiel Hernandez
"""

import serial
import sys
from serial.tools.list_ports import comports
import time

class Collectador():
    """
    This class is used for control of stepper motors. A serial connection is established with the microcontroller controlling the stepper motors.

    Attributes:
        baud (int): The baud rate the serial connection is using.
        port (string): the microcontrollers port for the serial connection.
        ser (serial Object): Instance of serial object representing the serial connection.
        serialConnected (bool): True/False of whether the serial connection was established.
        position (int): the position that the XY Stage is currently at.
        uniqueID (string): the unique identifier the microcontroller returns for automatic port selection.
        usingSnakePattern (bool): True/False whether the collection pattern is currently a snake pattern.
        currentPattern (string): string representation of the current pattern.
    """
    def __init__(self,my_port):
        """
        The constructor for the Collectador class.

        Parameters:
            my_port (string): the microcontrollers port for the serial connection.
        """

        self.baud = 9600
        self.port = my_port
        self.ser = None
        self.serialConnected = False
        self.position = None
        self.uniqueID = ""
        self.usingSnakePattern = True
        self.currentPattern = "Snake"

        self.serial_connect()

        #The sleep for a second is needed to wait for serial connection to be
        #connected so reset could be run.
        time.sleep(1)
        self.reset()

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
        print("Sent a serial command: %s %s"%("Collectador",cmd))

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

    def reset(self):
        """
        This method sends the command "Z" to the microcontroller which is programmed to move the XY Stage to the origin when receiving this command.
        Simulatenously also updates the current position of the XY Stage.
        """
        self.send('Z')
        self.position = 0
        #print(self.position)

    def eject(self):
        """
        This method sends the command "E" to the microcontroller which is programmed to move the XY Stage to the eject position when receiving this command.
        Simulatenously also updates the current position of the XY Stage.
        """
        self.send('E')
        self.position = 32
        #print(self.position)

    def next_site(self):
        """
        This method sends the command "N" to the microcontroller which is programmed to move the XY Stage to the next position when receiving this command.
        Simulatenously also updates the current position of the XY Stage.
        """
        if self.position < 31:
            self.send('N')
            self.position += 1
            #print(self.position)

    def last_site(self):
        """
        This method sends the command "L" to the microcontroller which is programmed to move the XY Stage to the last position when receiving this command.
        Simulatenously also updates the current position of the XY Stage.
        """
        if self.position > 0:
            self.send("L")
            self.position -= 1
            #print(self.position)

    def moveOneUp(self):
        """
        This method makes the top motor move one step up.
        """
        self.send("U")

    def moveOneDown(self):
        """
        This method makes the top motor move one step down.
        """
        self.send("I")

    def moveOneRight(self):
        """
        This method makes the bottom motor move one step to the right.
        """
        self.send("O")

    def moveOneLeft(self):
        """
        This method makes the bottom motor move one step to the left.
        """
        self.send("P")

    def setOrigin(self):
        """
        This method sets the origin once the user has manually moved it to his liking.
        Should be the upper left corner of well plate.
        """
        self.send("M")

    def toggle_pattern(self):
        """
        This method sends the command "X" to the microcontroller which is programmed to toggle the movement layout.
        The two movement patterns currently supported are snake pattern and top down pattern.
        """
        self.send("X")
        if self.usingSnakePattern:
            self.currentPattern = "Top Down"
            self.usingSnakePattern = False
        else:
            self.currentPattern = "Snake"
            self.usingSnakePattern = True

    def get_info(self):
        """
        This method sends the command "?" to the microcontroller which is programmed to send back a unique ID.
        The attribute uniqueID is set to the microcontrollers response.

        Returns:
            response (string): The unique ID that the microcontroller sends back through the serial connection.
        """
        self.ser.reset_input_buffer()
        self.send('?')
        response = self.ser.readline().decode()
        response = self.chop_return(response)
        self.uniqueID = response
        return response
