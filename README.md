# can-robust-industrial-card
Robust industrial fully isolated CAN bus node card, designed to be interfaced with a wood sanding machine PLC. 
The project contains two cards, basically the hardware is the same except the option moving some opto-couplers between the cards. The input card interfaces 46 input sensor, to sense the width of the wood piece in real time, while the output card interfaces 46 output air solenoid, to actuate them in realtime as well.
The cards are dsPIC33EP512MC506 based which is 3.3 voltage logic, the voltages are fully isolated among the input, output and controller areas. 

Hardware folder contains the Schematic and Layout designed on Altium designer. 
Software folder contains the input and output cards software codes built on MPLAB X IDE v3.26, C Compiler Version: XC16 v1.26 .

The can message is transmitted to/from CAN buffer directly to the RAM using DMA, CPU then is notified with the data presence to read it, or CPU writes the data to the RAM and notifies the DMA to write it to the CAN bus buffer.
