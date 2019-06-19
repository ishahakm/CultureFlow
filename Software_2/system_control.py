"""
Controller

This is where you operate on the model through the gui
"""
from model import CultureFlow_Model
from serial_helper_functions import get_ports, locate_device_comport

class CultureFlow_Control():
    """
    controller for original hardware set-up
    """
    def __init__(self,controller):

        print('CultureFlow_Control object created')

        self.model = CultureFlow_Model()
        self.avaiable_devices = {}
        self.view_message = "No updates available"
        self.controller = controller

    def load(self,screen=None):

        comports = get_ports()
        for device in self.model.device_ids:
            self.view_message = "sending call for %s to available comports"%device
            screen.update_text()

            result = locate_device_comport(comports,self.model.device_ids[device][0],self.model.device_ids[device][1])

            if result == "NC":
                self.view_message = "No devices found"
                screen.update_text()

            elif result == "NF":
                device_available = False
                self.view_message = "%s not found"%device
                screen.update_text()

            else:
                self.view_message = "%s found"%device
                screen.update_text()

                device_available = True
                comport = result[0]
                comports = result[1]

            if device_available:
                self.avaiable_devices[device] = True
            elif not device_available:
                self.avaiable_devices[device] = False
        print(self.avaiable_devices)
