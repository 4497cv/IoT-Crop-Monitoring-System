/*
    \file:   iotcrpmonitor.ino
    \brief:  This is source file for the implementation of the IoT crop monitor
    \author: César Villarreal @4497cv
    \date:   01/04/2020
*/

/****************************************
 * Include Libraries
 ****************************************/
#include "lib/UbidotsESPMQTT.h"
#include "lib/lm35dz.h"
#include "lib/cd4051be.h"
/****************************************
 * Define Constants
 ****************************************/
#define TOKEN          "BBFF-sCI4BMLnYxSCDgJwsR5AwQlwIqYsl6" // Your Ubidots TOKEN
#define WIFINAME_CASA  "ARRIS-AC62" //Your SSID
#define WIFIPASS_CASA  "F3DA7DAD49EFE77B" // Your Wifi Pass
#define WIFINAME       "The Coolest Pool Wifi"
#define WIFIPASS       "2WC466403315"
#define DEVICE_LABEL   "esp8266"
#define PWROUT_VALUE   62
#define TEMP_DELAY     60 //2 min delay
#define LUM_DELAY      100 //5 min delay
#define HUM_DELAY      240 //4 min delay

static uint8_t master_switch_st;
static float temp_delay;
static float lum_delay;
static float sm_delay;
static char* sw_topic = "/v1.6/devices/control/master-switch/lv";

Ubidots client(TOKEN);

void temperature_isr()
{
    float sensor_val;
    cd4051be_setChannel(CH_0);
    sensor_val = LM35DZ_get_current_temperature(celcius);
    
    if((temp_limit_low  <  sensor_val) &&
       (temp_limit_high >  sensor_val))
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

void callback(char* topic, byte* payload, unsigned int length)
{
  int i;
  Serial.print(topic);

  for (i = 0; i < length ; i++) Serial.print((char)payload[i]);
  Serial.println();

  if(topic == sw_topic)
  {
      Serial.printf("MS payload [0] = %f\n", payload[0]);
      master_switch_st = payload[0];
  }

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
  client.ubidotsSetBroker("industrial.api.ubidots.com");
  /* activate debugging messages on console */
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  /* establish wifi communication */
  client.wifiConnection(WIFINAME_CASA, WIFIPASS_CASA);
  /* set message callback function */
  client.begin(callback);
  /* subscribe to a device variable */
  client.ubidotsSubscribe("control","master-switch"); //Insert the dataSource and Variable's Labels
  
}

void loop()
{
 channel_e current_channel;
  float sensor_val;

  if(!client.connected())
  {
      client.reconnect();
  }


    current_channel = cd4051be_getCurrentChannel();

    switch(current_channel)
    {
      case CH_0:
          temperature_isr();
      break;
      case CH_1:
          luminosity_isr();
      break;
      case CH_2:
          soil_humidity_isr();
      break;
      default:

      break;
    }

  delay(30000);
  cd4051be_SwitchChannel();
  client.loop();
}
