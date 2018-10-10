#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <string.h>
#include <math.h>
#include <ESP8266WiFi.h>
#include <InfluxDb.h>

#define INFLUXDB_HOST "HOST"
#define INFLUXDB_PORT 0000
#define INFLUXDB_DATABASE "DATABSE"

const char* ssid     = "SSID";
const char* password = "PASSWORD";

InfluxDB    influx(INFLUXDB_HOST,INFLUXDB_PORT);


InfluxData  measurement("temperature_celsius");


void setup()
{
  influx.setDb(INFLUXDB_DATABASE);

  Serial.begin(115200); 
  delay(100);

  // * Connecting to specified WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // * WiFi connection FIN

}

void loop() {
  // ! of course this delay shouldn't be used, but for testing it's enough
  delay(1000);
  float temperature = random(25); //to test out the write to the InfluxDB temperature is just a random number

  //create a measurement object with the measurement-key called "temperature_celsius"
  //add data
  InfluxData  measurement("temperature_celsius");
  measurement.addTag("location", "building9");
  measurement.addTag("room", "kitchen");
  measurement.addValue("temperature", temperature);

  //write new data to the InfluxDB
  influx.write(measurement);
}
