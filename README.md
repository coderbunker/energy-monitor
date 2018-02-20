# energy-monitor
Project based on Open Energy Monitor https://openenergymonitor.org/

This project runs on the NodeMCU combined with the ADS1115 Analog Digital Converter and sends the Data to InfluxDB via UDP

The following link gives an example             
http://whatnicklife.blogspot.sg/2015/09/experimenting-with-energy-monitors.html

# NodeMCU Documentation

https://nodemcu.readthedocs.io/en/master/

# Using nodeMCU with Arduino IDE

Using the Arduino IDE to program the NodeMCU needs a setup. The following link describes how it's done.
https://www.marginallyclever.com/2017/02/setup-nodemcu-drivers-arduino-ide/


# Library installation
 
It needs the following libraries to work:
 
 *	Adafruit_ADS1X15 https://github.com/adafruit/Adafruit_ADS1X15
 *	ESP8266Wifi https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
 *	Wifi  
 *	Wire 
 
 Guide to install libraries https://www.arduino.cc/en/Guide/Libraries
 
# Bill of Materials
*	NodeMCU 
*	ADS1115
*	YHDC SCT-013-000 CT Sensor (Current Output)
*	3.5mm Headphone jack to PCB
*	33 Ohm 3 Watt Resistor
*	Jumper Wires

# Testing on a Breadbord

Put everything together according to the picture but use 33 Ohm Resistor instead of 330 Ohm Resistor!

[Circuit](https://github.com/coderbunker/energy-monitor/blob/master/circuit_design.jpg)
