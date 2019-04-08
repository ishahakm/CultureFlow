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
from decimal import Decimal

"""
When using this module errors can occur if:
more than one ISMATEC device connected at the same timeself.

myport = ThreePump(some_serial_port)
myport.port = new_port
myport.connect()

"""
class ThreePump():
    """
    This class is used for control of the Pump. A serial connection is established with the microcontroller controlling the Pump.

    Attributes:
        baud (int): The baud rate the serial connection is using.
        port (string): the microcontrollers port for the serial connection.
        ser (serial Object): Instance of serial object representing the serial connection.
        serialConnected (bool): True/False of whether the serial connection was established.
        calibrationvolume (int): amount of fluid used for calibration.
        calibrationunit (string): the unit for the amount of fluid used for calibration.
        isOn (bool): True/False on whether the pump is on or not.

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

        self.serial_connect()

        self.calibrationvolume = 1
        self.calibrationunit = "mL"
        self.setTargetCalibrationVolume()
        self.setDefaults()

        self.isOn = None
    def serial_connect(self):
        """
        This method establishes the serial connection with the microcontroller.

        Once the comport of the pump is known we open a serial connection to it using pySerial.
        """

        #print("This pump's port is: " + str(self.port) + " !!!!!!!!")

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
        print("Sent a serial command: %s %s "%("Pump",cmd))
    def send_return(self,cmd):
        """
        This method uses the serial connection opened instance of pump and sends the text written in cmd across that connection.

        Returns:
            response (string): The response or what the microcontroller returns.
        """
        self.ser.flushInput()
        self.ser.flushOutput()
        # sleep(1) for 100 millisecond delay
        # 100ms dely
        self.ser.write(cmd.encode('ascii') + '\r'.encode('ascii'))
        time.sleep(.1)
        print("Sent a serial command: %s %s "%("Pump",cmd))
        response = self.ser.readline().decode()
        response = self.chop_return(response)
        print(response)

        return response
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
    def FormatVolume(self,V,unit):
        """
        This method formats the volume.

        Parameters:
            V (int): The amount of volume.
            unit (string): The unit the volume should be set in.
        """
        if unit == "mL":
            S = 1
        elif unit == "uL":
            S = 10E-4
        A = '%E' % Decimal(str(float(V)*S))
        B = A[-3:]
        C = B[0]+B[2]
        D = str(int(float(A[:5])*1000))
        E = D+C
        return E
    def setFlow(self,channel,flowrate):
        """
        This method sets the flow rate of the pump.

        Parameters:
            channel (int): The channel whose flow rate will be set.
            flowrate (int): The flow rate which is to be set.
        """
        flowrate = self.FormatVolume(flowrate,"uL")
        print(flowrate)
        self.send(str(channel)+"f"+flowrate)
    def setDir(self,channel,direction):
        """
        This method sets the direction for the flow of fluid.

        Parameters:
            channel (int): The channel flow rate direction will be set.
            direction (string): The direction for the pump CW or CCW.
        """

        if direction == "CW":
            self.send(str(channel)+"J")
            self.send(str(channel)+"xRJ")

        if direction == "CCW":
            self.send(str(channel) + "K")
            self.send(str(channel) + "xRK")
    def start(self,channel):
        """
        This method sends a command to the microcontroller which is programmed to start the pump only for the channel chosen.

        Parameters:
            channel (int): the channel that will start.
        """
        self.send(str(channel)+"H")
    def stop(self,channel):
        """
        This method sends a command to the microcontroller which is programmed to stop the pump only for the channel chosen.

        Parameters:
            channel (int): the channel that will stop.
        """

        self.send(str(channel)+"I")
    def calibrate(self,channel):
        """
        This method sends a command to the microcontroller which is programmed to calibrate the pump only for the channel chosen.

        Parameters:
            channel (int): the channel that will be calibrated.
        """

        self.send(str(channel)+"xY")
    def abort_calibration(self,channel):
        """
        This method sends a command to the microcontroller which is programmed to abort calibration of the pump only for the channel chosen.

        Parameters:
            channel (int): the channel that will abort calibration.
        """

        self.send(str(channel)+"xZ")
    def setTargetCalibrationVolume(self):
        """
        This method sets the target calibration volume.
        """

        for i in range(3):
            to_send = str(i+1)+"x"+self.FormatVolume(self.calibrationvolume,self.calibrationunit)
            self.send(to_send)
    def setMeasuredVolume(self,channel,volume):
        """
        This method sets the measured volume.
        """

        volume = self.FormatVolume(volume,"mL")
        self.send_return(str(channel)+"xV"+volume)
    def start_all(self):
        """
        This method sends a command to the microcontroller which is programmed to start the pump for all channels.
        """
        self.send("0H")
    def stop_all(self):
        """
        This method sends a command to the microcontroller which is programmed to stop the pump for all the channels.
        """
        self.send("0I")
    def setDefaults(self):
        """
        This method sets the deafults for the pump.
        """
        # self.send_return('1M')
        # self.send_return('2M')
        # self.send_return('3M')

        self.send_return('0xM')
        self.send_return('0xM')
        self.send_return("0xU1000+0")
        self.send_return("0xW00003000")


        #setting to flowrate mode
        # for channel in range(3):
        #     self.send(str(channel+1)+"xU1000+0")
        #     self.send(str(channel+1)+"xW00002400")

        # self.send("1f")
        # self.send("2f")
        # self.send("3f")
def get_ports():
    """
    This method uses serial module's comports command to get and return serial ports.
    """
    ports = []
    for n, (port, desc, hwid) in enumerate(sorted(comports()), 1):
        ports.append(port)

    #added for fake serial testing

    ports.append("fakePump")
    ports.append("fakeMswitch")
    ports.append("fakeArduino")

    return ports
