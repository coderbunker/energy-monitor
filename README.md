# Energy-monitor

Project based on [Open Energy Monitor](https://openenergymonitor.org/)

This project runs on the NodeMCU combined with the ADS1115 Analog Digital Converter and sends the Data to InfluxDB via UDP

The following link gives a complete example : [Energy monitors](http://whatnicklife.blogspot.sg/2015/09/experimenting-with-energy-monitors.html)


## Setup Arduino and Server


### Arduino IDE
Setup the Arduino IDE to program the NodeMCU : [Link](https://www.marginallyclever.com/2017/02/setup-nodemcu-drivers-arduino-ide/)

Choose NodeMCU 1.0 ESP-12E Module for the board


### Libraries

It needs the following libraries to work:

 *	[Adafruit_ADS1X15](https://github.com/adafruit/Adafruit_ADS1X15)
 *	[ESP8266Wifi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)

Guide to install libraries https://www.arduino.cc/en/Guide/Libraries

### Server

1. Install InfluxDB : [Link](https://portal.influxdata.com/downloads)

```
$ sudo systemctl start influxdb
```

2. Active the HTTP connection : [Link](https://docs.gitlab.com/ee/administration/monitoring/performance/influxdb_configuration.html)

```
sudo nano -config /etc/influxdb/influxdb.conf
```

Change this :

```
[http]
  # Determines whether HTTP endpoint is enabled.
    enabled = true

  # The bind address used by the HTTP service.
    bind-address = ":8086"

  # Determines whether user authentication is enabled over HTTP/HTTPS.
    auth-enabled = false
```


3. Active the UDP connection : [Link](https://www.influxdata.com/blog/how-to-send-sensor-data-to-influxdb-from-an-arduino-uno/)

```
sudo nano -config /etc/influxdb/influxdb.conf
```

Change this :

```
[[udp]]
  enabled = true
  bind-address = ":8888"
  database = "arduino"
```

4. Create the InfluxDB DB :

```
influx

```
```
CREATE DATABASE arduino
```


5. Install Grafana : [Link](http://docs.grafana.org/installation/) [For Start](http://docs.grafana.org/guides/getting_started/)


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
