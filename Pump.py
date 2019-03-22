#import serial
import fakeSerial as serial
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

    def __init__(self,my_port):
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
        """Once the comport of the pump is known we open a serial connection
        to it using pySerial"""

        print("This pump's port is: " + str(self.port) + " !!!!!!!!")

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
        print("Sent a serial command: %s %s "%("Pump",cmd))
    def send_return(self,cmd):
        """
        uses serial connection opened instance of pump and sends
        the text written in cmd across that connection.
        Function had option
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
        """output of the pump comes with a carriage return and newline at the end of itself.
        We chop them off here.
        """
        if ret.endswith('\r\n'):
            ret = ret[:-2]
        return ret
    def FormatVolume(self,V,unit):
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
        flowrate = self.FormatVolume(flowrate,"uL")
        print(flowrate)
        self.send(str(channel)+"f"+flowrate)
    def setDir(self,channel,direction):
        if direction == "CW":
            self.send(str(channel)+"J")
        if direction == "CCW":
            self.send(str(channel)+"K")
    def start(self,channel):
        self.send(str(channel)+"H")
    def stop(self,channel):
        self.send(str(channel)+"I")
    def calibrate(self,channel):
        self.send(str(channel)+"xY")
    def abort_calibration(self,channel):
        self.send(str(channel)+"xZ")
    def setTargetCalibrationVolume(self):
        for i in range(3):
            to_send = str(i+1)+"x"+self.FormatVolume(self.calibrationvolume,self.calibrationunit)
            self.send(to_send)
    def setMeasuredVolume(self,channel,volume):
        volume = self.FormatVolume(volume,"mL")
        self.send(str(channel)+"xV"+volume)
    def start_all(self):
        for i in range(3):
            self.send(str(i+1)+"H")
        self.isOn = True
    def stop_all(self):
        for i in range(3):
            self.send(str(i+1)+"I")
        self.isOn = False
    def setDefaults(self):
        #setting to flowrate mode
        for channel in range(3):
            self.send(str(channel+1)+"xU1000+0")
            self.send(str(channel+1)+"xW00000600")
        self.send("M")
def get_ports():
    """
    Uses serial module's comports command to get and return serial ports
    """
    ports = []
    for n, (port, desc, hwid) in enumerate(sorted(comports()), 1):
        ports.append(port)

    #added for fake serial testing

    ports.append("fakePump")
    ports.append("fakeMswitch")
    ports.append("fakeArduino")

    return ports
