# picOLED Linux Driver

This is the Linux driver for the [picOLED](https://github.com/HughTB/picoled) project

## Getting Started

- Follow the guide on the picOLED main page
- Install the driver (Instructions coming soon)
- Ensure your user is part of the group which owns the serial port of the picOLED device (usually /dev/ttyACM0-99)
using `$ usermod -aG {group} {username}`. Typically, the serial port will belong to either `uucp` or `dialout` depending
upon your distro, you can check using `ls -l`. Some distros give access to these ports by default, so check before
making any changes

## Functionality

This driver is currently able to:
- Send data to the picOLED device
- Automatically search for and connect to the picOLED device
- Send CPU and Memory usage data
- Detect and send data from the CPU package temperature sensor (currently working for: Intel 6th Gen or newer; Zen 3
CPUs) (Other architectures will need to have the correct sensor information added to the detection routine)
