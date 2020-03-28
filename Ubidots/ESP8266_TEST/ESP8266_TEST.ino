/*
    \file   ESP8266_TEST.ino
    \author César Villarreal @4497cv
    \brief  
    \date   27/03/2020
*/

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
 * Define Constants
 ****************************************/
#define TOKEN         "BBFF-Sl9GbpiTrf6pVe93LsjEFK1tMr5ZCq" // Your Ubidots TOKEN
#define WIFINAME      "ARRIS-AC62" //Your SSID
#define WIFIPASS      "F3DA7DAD49EFE77B" // Your Wifi Pass
#define DEVICE_LABEL  "esp8266"
#define LM35DZ_PIN    0

Ubidots client(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length)
{
  int i;

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (i = 0; i < length ; i++)
  {
    Serial.print((char)payload[i]);
  }

  Serial.println();
}


static float LM35DZ_get_temperature_celsius(void)
{
  volatile float data;
  float celcius_temp;
  float v_out;
  //delay(500);
  /* read analog data from temperature sensor */
  data = analogRead(LM35DZ_PIN);
  /* The analog values read from the Arduino may have a value between 0 and 1024, 
   *  in which 0 corresponds to 0V and 1024 to 5V. So, we can easily get the output voltage of the sensor in mV */
  v_out = (data*5000)/1024;
  celcius_temp = v_out/10; /* 10 mv division*/
  return celcius_temp;
}

/****************************************
 * Main Functions
 ****************************************/

void setup()
{
  /* initialize serial communication */
  Serial.begin(115200);
  /* activate debug messages */
  client.setDebug(true);                            // Pass a true or false bool value to activate debug messages
  /* establish wifi connection */
  client.wifiConnection(WIFINAME, WIFIPASS);
  /* set callback */
  client.begin(callback);
  /* configure GPIO 0 */
  pinMode(LM35DZ_PIN, INPUT);
  /* set ubitdots device and subscribe to temperature label */
  client.ubidotsSubscribe("esp8266","temperature"); //Insert the dataSource and Variable's Labels
}

void loop()
{
  float sensorValue;
  int i;

  /* verify device conection to the internet*/
  if(!client.connected())
  {
      /* attempt to reconnect to wifi */
      client.reconnect();
      /* set ubitdots device and subscribe to temperature label */
      client.ubidotsSubscribe("esp8266","temperature"); //Insert the dataSource and Variable's Labels
  }
  else
  {

  
    client.add("temperature", LM35DZ_get_temperature_celsius());
    client.ubidotsPublish("esp8266");
    client.loop();
  }
}
