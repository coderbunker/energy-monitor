# Energy-monitor

Project based on [Open Energy Monitor](https://openenergymonitor.org/)

This project runs on the NodeMCU combined with the ADS1115 Analog Digital Converter and sends the Data to InfluxDB via UDP

The following link gives an example :

[Energy monitors](http://whatnicklife.blogspot.sg/2015/09/experimenting-with-energy-monitors.html)


## Setup


### Arduino IDE
Setup the Arduino IDE to program the NodeMCU : [Link](https://www.marginallyclever.com/2017/02/setup-nodemcu-drivers-arduino-ide/)

Choose NodeMCU 1.0 ESP-12E Module for the board


### Libraries

It needs the following libraries to work:

 *	[Adafruit_ADS1X15](https://github.com/adafruit/Adafruit_ADS1X15)
 *	[ESP8266Wifi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)
 
Guide to install libraries https://www.arduino.cc/en/Guide/Libraries

### Server

1. Install InfluxDB : [Link](https://www.influxdata.com/blog/how-to-send-sensor-data-to-influxdb-from-an-arduino-uno/)
2. Active the HTTP connection : [Link]() 
3. Active the UDP connection : [Link]()


## Hardware


### Bill of Materials
*	NodeMCU (ESP8266 Wifi)
*	ADS1115 
*	YHDC SCT-013-000 CT Sensor (Current Output)
*	3.5mm Headphone jack to PCB
*	33 Ohm 3 Watt Resistor
*	Jumper Wires

### Testing on a Breadbord

Put everything together according to the picture but use 33 Ohm Resistor instead of 330 Ohm Resistor!

[Circuit](https://github.com/coderbunker/energy-monitor/blob/master/circuit_design.jpg)

