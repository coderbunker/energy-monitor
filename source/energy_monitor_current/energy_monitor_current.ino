#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <string.h>
#include <math.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <InfluxDb.h>
//---------------------------------------------------------------------------------------
#define INFLUXDB_HOST "IP_OF_HOST"
#define INFLUXDB_PORT PORT_OF_HOST
#define INFLUXDB_DATABASE "NAME_OF_DATABASE"

const char* ssid     = "SSID";
const char* password = "PASSWORD";

#define INFLUXDB_LOCATION "CoderBunker"
#define INFLUXDB_ROOM     "ThreePhase"

WiFiUDP             udp;

Adafruit_ADS1115    ads(0x49);  // Declare an instance of the ADS1115

//int16_t rawADCvalue;  // The is where we store the value we receive from the ADS1115
float SCALEFACTOR = 0.125F; // This is the scale factor for the default +/- 4.096V Range we will use - see ADS1X15 Library
float MAX_CURRENT_COIL = 0.0707; // Maximum current of sensor rated at  Rms 100 A 
float MAX_VOLTAGE_ADC = 15.981; // Maximum Voltage on burden resistor calculated by burden resistor * maxAmps (33 Ohm * 0.0707 A)
float voltage_adc = 0.0; // The result of applying the scale factor to the raw value
float VOLTAGE_MAINS = 220.00; // line voltage
float current_coil = 0.0; // Calculated depending on the Voltage/Burden resistor 
float power;
float COIL_WINDING= 2000; // ratio of sensor 100:0.05

float sqV;
float sumV;
float sampleV;
float Vrms;

String line;
String PowerAsString;

void setup(void)
{
    Serial.begin(115200); 
    delay(100);
    
    // * Connecting to specified WiFi
    WiFi.begin(ssid, password);

    //WiFi.getNetworkInfo();
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Wifi Connected to" + String(ssid));
    // * WiFi connection FIN

    ads.setGain(GAIN_ONE);  
    
    ads.begin();

}

void loop(void)
{
    delay(500);
    voltage_adc = (calcVrms(64) )/1000.0;  // Sending Data every ~1.5 sec
    
    current_coil = ((voltage_adc) * MAX_CURRENT_COIL)/MAX_VOLTAGE_ADC;
    power = current_coil * COIL_WINDING * VOLTAGE_MAINS;

    String line = ("power_measurement,location=" + String(INFLUXDB_LOCATION) + ",room=" + String(INFLUXDB_ROOM) +" power=" + String(power) + ",voltage=" + String(voltage_adc));

    //Prepare Value-Package & Send it off to the Database
    Serial.println(line);
    udp.beginPacket(INFLUXDB_HOST,INFLUXDB_PORT);

    udp.print(line);
    udp.endPacket();
}

double calcVrms(unsigned int Number_of_Samples)
{
  
    for (unsigned int n = 0; n < Number_of_Samples; n++)
    {
        sampleV = ads.readADC_Differential_0_1();
        sqV = sampleV * sampleV;
        sumV += sqV;
    }
    
    Vrms = (sqrt(sumV / Number_of_Samples) * SCALEFACTOR) * sqrt(2);
    sumV = 0;    
    return Vrms;
}
