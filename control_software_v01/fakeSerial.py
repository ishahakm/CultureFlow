"""
Written by:
D. Thiebaut
"""

# a Serial class emulator
class Serial:
    """
    This class is used for simulation of an Arduino Serial Connection.

    Attributes:
        name (string): the name of the port.
        port (string): the microcontrollers port for the serial connection.
        baudrate (int): The baud rate the serial connection is using.
        timeout (int): maximum milliseconds to wait for serial data.
        parity (string): parity bit type.
        stopbits (int): amount of stop bits.
        xonxoff (int): whether xonxoff is on or off for flow control.
        rtscts (int): whether rtscts flow control is on or off.
    """
    def __init__( self, port='COM1', baudrate = 19200, timeout=1,
                  bytesize = 8, parity = 'N', stopbits = 1, xonxoff=0,
                  rtscts = 0):
        """
        The constructor for the Serial class.

        Parameters:
            port (string): the microcontrollers port for the serial connection. Defaulted to 'COM1'.
            baudrate (int): The baud rate the serial connection is using. Defaulted to 19200.
            timeout (int): maximum milliseconds to wait for serial data. Defaulted to 1.
            bytesize (int): The size of bytes. Defaulted to 8.
            parity (string): parity bit type. Defaulted to 'N'.
            stopbits (int): amount of stop bits. Defaulted to 1.
            xonxoff (int): whether xonxoff is on or off for flow control. Defaulted to 0.
            rtscts (int): whether rtscts flow control is on or off. Defaulted to 0
        """

        self.name     = port
        self.port     = port
        self.timeout  = timeout
        self.parity   = parity
        self.baudrate = baudrate
        self.bytesize = bytesize
        self.stopbits = stopbits
        self.xonxoff  = xonxoff
        self.rtscts   = rtscts
        self._isOpen  = True
        self._receivedData = ""
        self._data = "It was the best of times.\nIt was the worst of times.\n"

    def isOpen( self ):
        """
        This method returns True if the port to the arduino is open. False otherwise.

        Returns:
            _isOpen (bool): True/False depending if serial connection is open.
        """

        return self._isOpen

    def open( self ):
        """
        This method opens the port artificially.
        """

        self._isOpen = True

    def close( self ):
        """
        This method closes the port artificially.
        """

        self._isOpen = False

    def write( self, string ):
        """
        This method simulates writing a string of characters to the Arduino.

        Returns:
            An empty string.
        """

        print(string)
        #self._receivedData += string
        return ""

    def read( self, n=1 ):
        """
        This method simulates reading a pretermined amount of characters from the Arduino.

        Parameters:
            n (int): number of characters to read.

        Returns:
            s (string): the characters read from the Arduino.
        """

        s = self._data[0:n]
        self._data = self._data[n:]
        #print( "read: now self._data = ", self._data )
        return s

    def flush(self):
        """
        This method flushes the port artificially.
        """

        pass

    def flushInput(self):
        """
        This method flushes the input of the port artificially.
        """

        pass

    def flushOutput(self):
        """
        This method flushes the output of the port artificially.
        """

        pass

    def readline( self ):
        """
        This method simulates reading an entire line of characters until a "\n" is encountered.

        Returns:
            s (string): The line of characters read if there is one. Otherwise, an empty string.
        """

        returnIndex = self._data.index( "\n" )
        if returnIndex != -1:
            s = self._data[0:returnIndex+1]
            self._data = self._data[returnIndex+1:]
            return s
        else:
            return ""

    def __str__( self ):
        """
        This method creates a string representation of the Serial class.

        Returns:
            string: String representation of Serial class.
        """

        return  "Serial<id=0xa81c10, open=%s>( port='%s', baudrate=%d," \
               % ( str(self.isOpen), self.port, self.baudrate ) \
               + " bytesize=%d, parity='%s', stopbits=%d, xonxoff=%d, rtscts=%d)"\
               % ( self.bytesize, self.parity, self.stopbits, self.xonxoff,
                   self.rtscts )
