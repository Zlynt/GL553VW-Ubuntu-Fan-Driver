# Asus ROG GL553VW Ubuntu Fan Control

Control Asus ROG GL553VW Fan on Ubuntu 20.04 LTS.

## How to use

First enable the acpi_call module by runnin

 1. Enable the acpi_call module:
	 - `sudo modprobe acpi_call`
2. Run the program inside the build folder:
	- `sudo ./fan SPEED_OF_THE_FAN`



### Arguments

 - `sudo ./fan SPEED_OF_THE_FAN`

Where the SPEED_OF_THE_FAN could be an integer from 0 to 100:

|Value|Description                         |
|-|-------------------------------|
|0|Set the fan to automatic speed|
|100|Set the fan to the maximum speed|

For example, the command `sudo ./fan 0` sets the fan speed to automatic.
