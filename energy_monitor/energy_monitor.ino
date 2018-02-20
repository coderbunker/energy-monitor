#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <WiFiUdp.h>
#include <string.h>
#include <math.h>
#include <ESP8266WiFi.h>

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
float scalefactor = 0.125F; // This is the scale factor for the default +/- 4.096V Range we will use - see ADS1X15 Library
float maxAmps = 0.0707; // Maximum current of sensor rated at  Rms 100 A 
float maxVolts = 2.333; // Maximum Voltage on burden resistor calculated by burden resistor * maxAmps (33 Ohm * 0.0707 A)
float volts = 0.0; // The result of applying the scale factor to the raw value
float volts2 = 220.00; // line voltage
float amps = 0.0; // Calculated depending on the Voltage/Burden resistor 
float amps2 = 0; // Current on the line
float power;
float winding = 2000; // ratio of sensor 100:0.05

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
  
  // We start by connecting to a WiFi network

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

  udp.begin(port);
}

void loop(void)
{  

   
  volts = (calcVrms(2048) )/1000.0;  // Sending Data every ~6 sec
  //volts = (calcVrms(1024) )/1000.0; // Sending Data every ~3 sec //Need testing to see which amount of samples is still accurate enough
  //volts = (calcVrms(512) )/1000.0;  // Sending Data every ~1.5 sec
  amps = ((volts) * maxAmps)/maxVolts;
  amps2 = amps * winding;
  power = amps2 * volts2;

  PowerAsString = String  (power);
  line = String ("power value=" + PowerAsString);
  

  // Serial Data Output
    
//  Serial.print("Voltage Measured = ");
//  Serial.println(volts,6);
//  Serial.println();
//  Serial.print("amps = "); 
//  Serial.print(amps,6); 
//  Serial.print("\tamps2 = ");
//  Serial.println(amps2,6);
//  Serial.println();
//  Serial.print("Power = ");
//  Serial.println(power,6);
//  Serial.println();
//  Serial.println("Sending UDP packet...");

  // send the udp packet

  udp.beginPacket(host, port);
  udp.print(line);
  udp.endPacket();

//Detect runtime

//runtime = millis();
//Serial.println (runtime);
  
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
  
  Vrms = (sqrt(sumV / Number_of_Samples)* scalefactor)* sqrt(2);

  //Reset accumulators
  sumV = 0;
//--------------------------------------------------------------------------------------       
 
  return Vrms;
}
