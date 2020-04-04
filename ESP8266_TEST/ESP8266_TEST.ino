/*
    \file:   iotcrpmonitor.ino
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
#include <Ticker.h>  
/****************************************
 * Define Constants
 ****************************************/
#define TOKEN          "BBFF-Sl9GbpiTrf6pVe93LsjEFK1tMr5ZCq" 
#define DEVICE_LABEL   "node-mcu"

#define WIFINAME_CASA  "ARRIS-AC62"              //WIFI SSID
#define WIFIPASS_CASA  "F3DA7DAD49EFE77B"        // WIFI PWD
#define WIFINAME       "The Coolest Pool Wifi"
#define WIFIPASS       "2WC466403315"

#define PWROUT_VALUE   62
#define TEMP_DELAY     120 //2 min delay
#define LUM_DELAY      300 //5 min delay
#define HUM_DELAY      240 //4 min delay

static uint8_t master_switch_st;
static uint8_t temp_delay;
static uint8_t lum_delay;
static uint8_t sm_delay;
static uint8_t ticker0_st;
static uint8_t ticker1_st;
static uint8_t ticker2_st;
String sw_topic = String("/v1.6/devices/control/master-switch/lv");
String tempd_topic = String("/v1.6/devices/control/temp_delay/lv");
String humd_topic = String("/v1.6/devices/control/sm_delay/lv");
String lumd_topic = String("/v1.6/devices/control/lum_delay/lv");

Ubidots client(TOKEN);

Ticker ticker0, ticker1, ticker2;

void temperature_isr()
{
    float sensor_val;
    /* set channel 0 in multiplexor */
    cd4051be_setChannel(CH_0);
    /* retreive the current temperature value */
    sensor_val = LM35DZ_get_current_temperature(celcius);
    /* verify if value in the valid temperature range */
    if((temp_limit_low  <  sensor_val) &&
       (temp_limit_high >  sensor_val))
    {
        /* publish current temperature value to node mcu */
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
  String topic_str = String(topic);
   Serial.print("~~~~~~~~~~~~~~~~~~~~\n");
   Serial.print(topic);
   Serial.println();
  if(topic_str.equals(sw_topic))
  {
      master_switch_st = String((char*)payload).toInt();
      
      if(master_switch_st == 1)
      {
        Serial.print("Master Switch is ON\n");
      }
      else
      {
        Serial.print("Master Switch is OFF\n");
      }
  }
  else if(topic_str.equals(tempd_topic))
  {
    temp_delay = String((char)payload[0]).toInt();
  }
  else if(topic_str.equals(lumd_topic))
  {
    lum_delay = String((char)payload[0]).toInt();
  }
  else if(topic_str.equals(humd_topic))
  {
    sm_delay = (String((char)payload[0]).toInt());
  }
  
  if((temp_delay == 0) || (master_switch_st == 0))
  {
    Serial.print("Temperature ticker detached \n");
    ticker0.detach();
    ticker0_st = 0;
  }
  else if((ticker0_st == 0) && (master_switch_st == 1))
  {
    Serial.printf("Temperature ticker attached: %i minutes \n", temp_delay);
    ticker0.attach(temp_delay*60, temperature_isr);
    ticker0_st = 1;
  }
  
  if((lum_delay == 0) || (master_switch_st == 0))
  {
    Serial.print("Luminosity ticker detached \n");
    ticker1.detach();
    ticker1_st = 0;
  }
  else if((ticker1_st == 0) && (master_switch_st == 1))
  {
    Serial.printf("Luminosity ticker attached: %i minutes \n", lum_delay);
    ticker1.attach(lum_delay*60, luminosity_isr);
    ticker1_st = 1; 
  }  

  if((sm_delay == 0) || (master_switch_st == 0))
  {
    Serial.print("Soil moisture ticker detached \n");
    ticker2.detach();
    ticker2_st = 0; 
  }
  else if((ticker2_st == 0) && (master_switch_st == 1))
  {
    Serial.printf("Soil moisture ticker attached: %i minutes \n", sm_delay);
    ticker2.attach(sm_delay*60,  soil_humidity_isr);
    ticker2_st = 1;
  }
  Serial.print("~~~~~~~~~~~~~~~~~~~~\n");
  client.loop();
}

/****************************************
 * Main Functions
 ****************************************/
void setup()
{
  lm35dz_t sensor_config;
  /* set temperature sensor operation mode */
  sensor_config.op_mode = cd4051be;
  temp_delay = 120;
  lum_delay =  120;
  sm_delay =   120;
  ticker0_st = 0;
  ticker1_st = 0;
  ticker2_st = 0;
  /* initialize temperature sensor */ 
  LM35DZ_init(sensor_config);
  /* multiplexer module initialization */
  cd4051be_init();

  /* establish serial communication */
  Serial.begin(115200);
  /* establish mqtt broker (needed for mqtt subscribe) */
  client.ubidotsSetBroker("industrial.api.ubidots.com");
  /* activate debugging messages on console */
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  /* establish wifi communication */
  client.wifiConnection(WIFINAME_CASA, WIFIPASS_CASA);
  /* set message callback function */
  client.begin(callback);
  /* subscribe to a device variable */
  client.ubidotsSubscribe("control","temp_delay"); //Insert the dataSource and Variable's Labels
  client.ubidotsSubscribe("control","lum_delay"); //Insert the dataSource and Variable's Labels
  client.ubidotsSubscribe("control","sm_delay"); //Insert the dataSource and Variable's Labels
  client.ubidotsSubscribe("control","master-switch"); //Insert the dataSource and Variable's Labels
}

void loop()
{
  if(!client.connected())
  {
      client.reconnect();
      client.ubidotsSubscribe("control","temp_delay"); //Insert the dataSource and Variable's Labels
      client.ubidotsSubscribe("control","lum_delay"); //Insert the dataSource and Variable's Labels
      client.ubidotsSubscribe("control","sm_delay"); //Insert the dataSource and Variable's Labels
      client.ubidotsSubscribe("control","master-switch"); //Insert the dataSource and Variable's Labels
  }

  client.loop();
  delay(300);
}
