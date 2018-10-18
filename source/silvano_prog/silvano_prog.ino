#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <string.h>
#include <math.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include <SparkFun_Si7021_Breakout_Library.h>
#include <Adafruit_CCS811.h>

//---------------------------------------------------------------------------------------
#define INFLUXDB_HOST "10.1.0.230"
#define INFLUXDB_PORT 6969
#define INFLUXDB_DATABASE "test"

#define INFLUXDB_LOCATION "CoderBunker"
#define INFLUXDB_ROOM     "ThreePhase"

const char* ssid     = "Agora Space";
const char* password = "getstuffdone";

Ticker tick;

bool tickOccured;


void timerCallback() {

      tickOccured = true;

}

void user_init(void)
{
    tick.attach(1, timerCallback);
}


WiFiUDP             udp;

Adafruit_ADS1115    ADC_1(0x49);  // Declare an instance of the ADS1115 for 1st and 2nd phase

Adafruit_ADS1115    ADC_2(0x48);  // Declare an instance of the ADS1115 for 3rd phase

Weather sensor; //Create Instance of SI7021 temp and humidity sensor

Adafruit_CCS811 ccs;

//int16_t rawADCvalue;  // The is where we store the value we receive from the ADS1115
float SCALEFACTOR = 0.125F; // This is the scale factor for the default +/- 4.096V Range we will use - see ADS1X15 Library
//float MAX_CURRENT_COIL = 0.0707; // Maximum current of sensor rated at  Rms 100 A 
//float MAX_VOLTAGE_ADC = 15.981; // Maximum Voltage on burden resistor calculated by burden resistor * maxAmps (33 Ohm * 0.0707 A)
float burden_resistor=220;
float voltage_adc_1 = 0.0; // The result of applying the scale factor to the raw value
float voltage_adc_2 = 0.0;
float voltage_adc_3 = 0.0;
float VOLTAGE_MAINS = 220.00; // line voltage
float current_coil = 0.0; // Calculated depending on the Voltage/Burden resistor 
float power_1 = 0.0;
float power_2 = 0.0;
float power_3 = 0.0;
float Pges = 0.0;
float COIL_WINDING= 2000; // ratio of sensor 100:0.05


float humidity = 0;
float temp = 0;

float eCO2 = 0;
float TVOC = 0;

String line;
String PowerAsString;

void setup(void)
{
    Wire.setClock(400000);
    Serial.begin(115200); 
    delay(250);
  
    // * Connecting to specified WiFi
    WiFi.begin(ssid, password);

    //WiFi.getNetworkInfo();
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Wifi Connected to" + String(ssid));
    // * WiFi connection FIN

    ADC_1.setGain(GAIN_ONE);  
    ADC_2.setGain(GAIN_ONE);
    
    ADC_1.begin();
    ADC_2.begin();
    user_init();
    sensor.begin();

    if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
    }
    while(!ccs.available());
    float temp = ccs.calculateTemperature();
    ccs.setTempOffset(temp - 25.0);
}

void loop(void)
{
    if(tickOccured)
    {
        tickOccured = false;

        String line = ("power_measurement,location=" + String(INFLUXDB_LOCATION) + ",room=" + String(INFLUXDB_ROOM) + " power1=" + String(power_1) +",power2=" + String(power_2)+
        ",power3=" + String(power_3) + ",Pges=" + String(Pges) + ",humidity=" + String(humidity) + ",temp=" + String(temp) + ",eCO2=" + String(eCO2) + ",TVOC=" + String(TVOC));
        
        //Prepare Value-Package & Send it off to the Database
        Serial.println(line);
        udp.beginPacket(INFLUXDB_HOST,INFLUXDB_PORT);

        udp.print(line);
        udp.endPacket();
    }
    humidity = sensor.getRH();
    temp = sensor.getTemp();
    voltage_adc_1 = (calcVrms(32,1) )/1000.0;
    voltage_adc_2 = (calcVrms(32,2) )/1000.0;
    voltage_adc_3 = (calcVrms(32,3) )/1000.0;
    /*Serial.println(voltage_adc_1);
    Serial.println(voltage_adc_2);
    Serial.println(voltage_adc_3);*/

    if(ccs.available()){
    //float temp = ccs.calculateTemperature();
    if(!ccs.readData()){
      eCO2=ccs.geteCO2();
      TVOC=ccs.getTVOC();
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
//  current_coil = ((voltage_adc_1) * MAX_CURRENT_COIL)/MAX_VOLTAGE_ADC;
//  power = current_coil * COIL_WINDING * VOLTAGE_MAINS;
  power_1= calcPower(voltage_adc_1); 
  power_2= calcPower(voltage_adc_2);
  power_3= calcPower(voltage_adc_3);
  Pges=power_1+power_2+power_3;
}

double calcVrms(unsigned int Number_of_Samples, unsigned int phase)
{
    float Vrms;
    float sqV;
    float sumV;
    float sampleV;
    //Serial.println(phase);
    for (unsigned int n = 0; n < Number_of_Samples; n++)
    {
        switch(phase){
          case 1:{
            sampleV = ADC_1.readADC_Differential_0_1();
            //Serial.println("phase 1");
            break;
          }
          case 2:{
            sampleV = ADC_1.readADC_Differential_2_3();
            //Serial.println("phase 2");
            break;
          }
          case 3:{
            sampleV = ADC_2.readADC_Differential_0_1();
            //Serial.println("phase 3");
            break;
          }
          default: break;
        }
        sqV = sampleV * sampleV;
        sumV += sqV;
    }
    
    Vrms = (sqrt(sumV / Number_of_Samples) * SCALEFACTOR) * sqrt(2);
    sumV = 0;    
    return Vrms;
}

double calcPower(float voltage){
  double power;
  power=voltage/burden_resistor*COIL_WINDING*VOLTAGE_MAINS;
  return power;
}
