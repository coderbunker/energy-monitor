#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <WiFiUdp.h>
#include <string.h>
#include <math.h>
#include <ESP8266WiFi.h>
#include <InfluxDb.h>
//WIFI credentials go here
const char* ssid     = "Agora Space";
const char* password = "getstuffdone";


// the IP address of your InfluxDB host
byte host[] = {139, 198, 177, 222};

// the port that the InfluxDB UDP plugin is listening on
int port = 8089;

WiFiUDP udp;

Adafruit_ADS1115 ads;  // Declare an instance of the ADS1115

//int16_t rawADCvalue;  // The is where we store the value we receive from the ADS1115
float SCALEFACTOR = 0.125F; // This is the scale factor for the default +/- 4.096V Range we will use - see ADS1X15 Library
float MAX_CURRENT_COIL = 0.0707; // Maximum current of sensor rated at  Rms 100 A 
float MAX_VOLTAGE_ADC = 2.333; // Maximum Voltage on burden resistor calculated by burden resistor * maxAmps (33 Ohm * 0.0707 A)
float voltage_adc = 0.0; // The result of applying the scale factor to the raw value
float VOLTAGE_MAINS = 220.00; // line voltage
float current_coil = 0.0; // Calculated depending on the Voltage/Burden resistor 
float power;
float COIL_WINDING= 2000; // ratio of sensor 100:0.05

float sqV;
float sumV;
float sampleV;
float Vrms;

long runtime=0;

String line;
String PowerAsString;

void setup(void)
{
  Serial.begin(115200); 
  delay(100);
  
  // * Connecting to specified WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  WiFi.getNetworkInfo();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // * WiFi connection FIN


   // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  
  
  ads.setGain(GAIN_ONE);  
  
  ads.begin();

  //udp.begin(port);
}

void loop(void)
{
  voltage_adc = (calcVrms(256) )/1000.0;  // Sending Data every ~1.5 sec
  current_coil = ((voltage_adc) * MAX_CURRENT_COIL)/MAX_VOLTAGE_ADC;
  power = current_coil * COIL_WINDING * VOLTAGE_MAINS;

  PowerAsString = String  (power);
  line = String ("power value=" + PowerAsString);

  Serial.println(line);
}

double calcVrms(unsigned int Number_of_Samples)
{
  
  for (unsigned int n = 0; n < Number_of_Samples; n++)
  {
    sampleV = ads.readADC_Differential_0_1();

    // Root-mean-square method voltage
    // 1) square voltage values
    sqV = sampleV * sampleV;
    // 2) sum 
    sumV += sqV;
  }
  
  Vrms = (sqrt(sumV / Number_of_Samples)* SCALEFACTOR)* sqrt(2);

  //Reset accumulators
  sumV = 0;
//--------------------------------------------------------------------------------------       
 
  return Vrms;
}
