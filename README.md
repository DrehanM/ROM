# Room Occupancy Monitor
## Overview
The Room Occupancy Monitor is a device designed to record the occupancy of a given room with accuracy, transmit the occupancy data to a remote operating device via WiFi, and display the results to be accessed without having to enter the room.

## Motivation
I designed, built, and coded this device at the University of Bremen COMNETS department in the Summer 2017. The intention was to create a small, cheap, convenient device to accurately detect and report the occupancy of environments with moderate traffic by attaching to doorways. Upon completion, members of COMNETS plan to study and test how the device interacts as a node in the local wireless network.

## Features
The ROM is split into three main stages:
- Detection
- Transmission
- Display

Detection & transmission are handled by **UpdatedUDPoccu.ino** and the reception & display is handled by the **SocketDisplay.py**.

The overall design is focused on convenience and economic efficiency. The ROM hardware consists only of two IR sensors, the microcontroller, and connecting wires. As such, the total cost is less than $40, compared to typical occupancy sensors on the market that are well over $100 in price.

###### Shanal Drehan Mahaarachchi - UC Berkeley EECS Class of 2020.


