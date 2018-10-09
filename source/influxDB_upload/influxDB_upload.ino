#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <string.h>
#include <math.h>
#include <ESP8266WiFi.h>
#include <InfluxDb.h>

const char* ssid     = "Agora Space";
const char* password = "getstuffdone";

#define INFLUXDB_HOST "206.189.45.190"
#define INFLUXDB_PORT 8086
#define INFLUXDB_DATABASE "test"

InfluxDB    influx(INFLUXDB_HOST,INFLUXDB_PORT);


InfluxData  measurement("temperature_celsius");


void setup()
{
  influx.setDb(INFLUXDB_DATABASE);

  Serial.begin(115200); 
  delay(100);

  // * Connecting to specified WiFi
  Serial.println();
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
  // put your main code here, to run repeatedly:
  delay(1000);
  float temperature = random(25);

  InfluxData  measurement("temperature_celsius");
  measurement.addTag("location", "building9");
  measurement.addTag("room", "kitchen");
  measurement.addValue("temperature", temperature);

  // write it into db
  influx.write(measurement);
}
