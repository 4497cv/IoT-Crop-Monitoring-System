/****************************************
 * Include Libraries
 ****************************************/
#include <OneWire.h>
#include <DallasTemperature.h>
#include "UbidotsESPMQTT.h"
#include "lm35dz.h"
#include "cd4051be.h"
/****************************************
 * Define Constants
 ****************************************/
#define TOKEN          "BBFF-Sl9GbpiTrf6pVe93LsjEFK1tMr5ZCq" // Your Ubidots TOKEN
#define WIFINAME       "ARRIS-AC62" //Your SSID
#define WIFIPASS       "F3DA7DAD49EFE77B" // Your Wifi Pass
#define DEVICE_LABEL   "esp8266"
#define PWROUT_VALUE   62

static float temp_read;
static uint8_t deviceCount;
Ubidots client(TOKEN);
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(D3);  
// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);
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
  /* multiplexer module initialization */
  cd4051be_init();
  sensors.begin();
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

  deviceCount = 0;
  // locate devices on the bus
  Serial.println("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
}

void loop()
{
  channel_e current_channel;
  float sensor_val;
  
  if(!client.connected())
  {
      client.reconnect();
  }

  if(verifyPowerActive())
  {
      current_channel = cd4051be_getCurrentChannel();
      
      switch(current_channel)
      {
        case CH_0:
            cd4051be_setChannel(CH_0);
            sensor_val = get_current_temperature_celcius();
            client.add("lm35", sensor_val);
            client.ubidotsPublish("node-mcu");
        break;
        case CH_1:
            sensor_val = cd4051be_ChannelRead(CH_1);
            sensor_val = (sensor_val*100.0)/1024.0;
            client.add("luminosity", sensor_val);
            client.ubidotsPublish("node-mcu");
        break;
        case CH_2:
            sensor_val = cd4051be_ChannelRead(CH_2);
            sensor_val = (841-sensor_val)/4.05;
            client.add("soil-humidity", sensor_val);
            client.ubidotsPublish("node-mcu");
        break;
        default:
            
        break;
      }
      
      if((current_channel == CH_2) && deviceCount > 0)
      {
        sensors.requestTemperatures();
        sensor_val = sensors.getTempCByIndex(0);
        client.add("soil-temp", sensor_val);
        client.ubidotsPublish("node-mcu");
      }
    
      client.loop();
      cd4051be_SwitchChannel();
  }

  delay(20000);
}
