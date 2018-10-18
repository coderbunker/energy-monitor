#include <string.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
//---------------------------------------------------------------------------------------
const char* ssid     = "SSID";
const char* password = "PASSWORD";

uint8_t mac[6];

String MacToString(const uint8_t* mac);


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
  }

  WiFi.macAddress(mac);

  Serial.println(MacToString(mac));
}

void loop() {

}

String MacToString(const uint8_t* mac)
{
    String s_mac = "";
    for(int8_t index = 5 ; index >= 0 ; index--)
    {
        s_mac += String(mac[index],HEX);
        if(index != 0)
        {
            s_mac += ":";
        }
    }
    return s_mac;
}