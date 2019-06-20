# CultureFlow
##What is CultureFlow?
For starters it's a fluid handling platform that allows the user to run experiements using the organ-on-a-chip platform developed at Agarwal lab. The current components are are an ISMATEC Reglo peristaltic pump, Titan EX manifold switch, and a custom collection system also designed at Agarwal Lab. However, the state of this system is amenable to your needs! The control software was designed so that one can easily integrate any component that sends and recieves serial connections. So easy in fact, an undergrad can do it! ;) 

Steps to add a new component. (details coming soon)
1. Write python module for device specific function as per the new_device rubric
2. Add the device to the model.py file
3. You now have access to the device via system_control.py! Write whatever functions you need fo your device
4. Run the functions from the GUI, creating buttons and entries where needed. 
