# CultureFlow
![alt text](https://pubs.rsc.org/image/article/2019/ay/c9ay01663e/c9ay01663e-f1_hi-res.gif)

## What is CultureFlow?

For starters it's a fluid handling platform that allows the user to run experiements using the organ-on-a-chip platform developed at Agarwal lab. The current components are are an ISMATEC Reglo peristaltic pump, Titan EX manifold switch, and a custom collection system also designed at Agarwal Lab. You'll find all documentation including electronics in this However, the state of this system is amenable to your needs! The control software was designed so that one can easily integrate any component that sends and recieves serial connections.

## Steps to add a new component.
1. Write python module for device specific function as per the new_device rubric
2. Add the device to the model.py file
3. You now have access to the device via system_control.py! Write whatever functions you need fo your device
4. call the functions from the GUI via buttons and entries where needed.

# Citation
For more information check out our publication: https://doi.org/10.1039/C9AY01663E

If you use components of CultureFlow in your work, please cite the publication as follows:

Ishahak, Matthew, Liev Birman, Daniel Carbonero, Jordan Hill, Adiel Hernandez, Siddarth Rawal, and Ashutosh Agarwal. "Integrated platform for operating and interrogating organs-on-chips." Analytical methods 11, no. 43 (2019): 5645-5651.
