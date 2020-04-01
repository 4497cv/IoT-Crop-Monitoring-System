/*
    \file:   iotcrpmonitor.cpp
    \brief:  This is source file for the implementation of the IoT crop monitor
    \author: César Villarreal @4497cv
    \date:   01/04/2020
*/
/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"
#include "lm35dz.h"
#include "cd4051be.h"
#include <Ticker.h>  //Ticker Library
/****************************************
 * Define Constants
 ****************************************/
#define TOKEN          "BBFF-Sl9GbpiTrf6pVe93LsjEFK1tMr5ZCq" // Your Ubidots TOKEN
#define WIFINAME       "ARRIS-AC62" //Your SSID
#define WIFIPASS       "F3DA7DAD49EFE77B" // Your Wifi Pass
#define DEVICE_LABEL   "esp8266"
#define PWROUT_VALUE   62
#define TEMP_DELAY     60  //1 min delay
#define LUM_DELAY      300 //5 min delay
#define HUM_DELAY      120 //2 min delay

static float temp_read;

Ubidots client(TOKEN);

Ticker ticker0, ticker1, ticker2;

void temperature_isr()
{
    float sensor_val;
    cd4051be_setChannel(CH_0);
    sensor_val = LM35DZ_get_current_temperature(celcius);
    if((temp_limit_low < sensor_val) && (temp_limit_high > sensor_val))
    {
        client.add("lm35", sensor_val);
        client.ubidotsPublish("node-mcu");  
    }
    client.loop();
}

void luminosity_isr()
{
    float sensor_val;
    sensor_val = cd4051be_ChannelRead(CH_1);
    sensor_val = (sensor_val*100.0)/1024.0;
    client.add("luminosity", sensor_val);
    client.ubidotsPublish("node-mcu");
    client.loop();
}

void soil_humidity_isr()
{
    float sensor_val;
    sensor_val = cd4051be_ChannelRead(CH_2);
    sensor_val = (841-sensor_val)/4.05;
    client.add("soil-humidity", sensor_val);
    client.ubidotsPublish("node-mcu");
    client.loop();
}
/****************************************
 * Auxiliar Functions
 ****************************************/
uint8_t verifyPowerActive(void)
{
  uint8_t flag;
  float sensor_val;
  
  sensor_val = cd4051be_ChannelRead(CH_1);
  sensor_val = (sensor_val*100.0)/1024.0;
  sensor_val = (uint8_t) sensor_val;
  
  if(PWROUT_VALUE == sensor_val)
  {
    flag = 0;
    Serial.print("Power is not active\n");
  }
  else
  {
    flag = 1;
  }

  return flag;
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++)
  {
    Serial.print((char)payload[i]);
  }

  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/
void setup()
{
  lm35dz_t sensor_config;
  sensor_config.op_mode = cd4051be;
  
  LM35DZ_init(sensor_config);
  /* multiplexer module initialization */
  cd4051be_init();

  /* establish serial communication */
  Serial.begin(115200);
  /* activate debugging messages on console */
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  /* establish wifi communication */
  client.wifiConnection(WIFINAME, WIFIPASS);
  /* set message callback function */
  client.begin(callback);
  /* subscribe to a device variable */
  client.ubidotsSubscribe("node-mcu","temperature"); //Insert the dataSource and Variable's Labels
  //client.ubidotsPublish("switch");
  ticker0.attach(TEMP_DELAY, temperature_isr);
  ticker1.attach(LUM_DELAY,  luminosity_isr);
  ticker2.attach(HUM_DELAY,  soil_humidity_isr);
}

void loop()
{
  if(!client.connected())
  {
      client.reconnect();
  }
}
