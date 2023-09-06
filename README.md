# PicOLED Linux Driver

This is the Linux driver for the [PicOLED](https://github.com/HughTB/picoled) project

## Getting Started

- Follow the guide on the PicOLED main page
- Install the driver (Instructions coming soon)
- Ensure your user is part of the group which owns the serial port of the PicOLED device (usually /dev/ttyACMXX)
using `$ usermod -a -G uucp {username}`. Typically, the serial port will belong to either `uucp` or `dialout` depending
upon your distro, you can check using `ls -l`

## Functionality

This driver is currently able to:
- Send data to the PicOLED device
- Automatically search for and connect to the PicOLED device
- Send CPU and Memory usage data
- Detect and send data from the CPU package temperature sensor (currently working for: Intel 8th Gen or newer; Zen 3
CPUs) (Other architectures will need to have the correct sensor information added to the detection routine)
