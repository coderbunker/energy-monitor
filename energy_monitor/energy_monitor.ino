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

int16_t rawADCvalue;  // The is where we store the value we receive from the ADS1115
float scalefactor = 0.125F; // This is the scale factor for the default +/- 6.144 Volt Range we will use
float maxAmps = 0.0707; // Maximum current of sensor rated at  Rms 100 A 
float maxVolts = 2.333; // Maximum Voltage on burden resistor calculated by burden resistor * maxAmps (33 Ohm * 0.0707 A)
float volts = 0.0; // The result of applying the scale factor to the raw value
float volts2 = 220.00;
float amps = 0.0; // Calculated depending on the Voltage/Burden resistor 
float amps2 = 0; // Current on the line
float power;
float winding = 2000; // ratio of sensor 100:0.05

float offsetV;
float filteredV;
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
  
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  ads.begin();

  udp.begin(port);
}

void loop(void)
{  

  //rawADCvalue = ads.readADC_Differential_0_1(); 
  volts = (calcVrms(2048) )/1000.0;
  amps = ((volts) * maxAmps)/maxVolts;
  amps2 = amps * winding;
  power = amps2 * volts2;

  PowerAsString = String  (power);
  line = String ("power value=" + PowerAsString);
  //line = (power);
  
  //Serial.print("Raw ADC Value = "); 
  //Serial.print(rawADCvalue); 
  Serial.print("Voltage Measured = ");
  Serial.println(volts,6);
  Serial.println();
  Serial.print("amps = "); 
  Serial.print(amps,6); 
  Serial.print("\tamps2 = ");
  Serial.println(amps2,6);
  Serial.println();
  Serial.print("Power = ");
  Serial.println(power,6);
  Serial.println();


  // send the packet
  Serial.println("Sending UDP packet...");
  udp.beginPacket(host, port);
  udp.print(line);
  udp.endPacket();
  
}

double calcVrms(unsigned int Number_of_Samples)
{
  
  for (unsigned int n = 0; n < Number_of_Samples; n++)
  {
    sampleV = ads.readADC_Differential_0_1();

    // Root-mean-square method current
    // 1) square current values
    //sqV = filteredV * filteredV;
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
