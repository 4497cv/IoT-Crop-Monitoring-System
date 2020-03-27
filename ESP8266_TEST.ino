/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
 * Define Constants
 ****************************************/
#define TOKEN "BBFF-Sl9GbpiTrf6pVe93LsjEFK1tMr5ZCq" // Your Ubidots TOKEN
#define WIFINAME "ARRIS-AC62" //Your SSID
#define WIFIPASS "F3DA7DAD49EFE77B" // Your Wifi Pass
#define DEVICE_LABEL "esp8266"

Ubidots client(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  if ((char)payload[0]=='1'){
    digitalWrite(16, HIGH);
  }
  else{
    digitalWrite(16, LOW);
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  pinMode(16, OUTPUT);
  client.ubidotsSubscribe("esp8266","temperature"); //Insert the dataSource and Variable's Labels
}

void loop()
{
  // put your main code here, to run repeatedly:
  if(!client.connected())
  {
      client.reconnect();
      client.ubidotsSubscribe("esp8266","temperature"); //Insert the dataSource and Variable's Labels
  }
  
  client.add("temperature", 10);
  client.ubidotsPublish("esp8266");
  client.loop();
}
