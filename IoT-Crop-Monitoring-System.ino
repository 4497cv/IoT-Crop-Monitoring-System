/*
    \file:   iotcrpmonitor.ino
    \brief:  This is source file for the implementation of the IoT crop monitor
    \author: César Villarreal @4497cv
    \date:   01/04/2020
*/

/****************************************
 * Include Libraries
 ****************************************/
#include "lib/lm35dz.h"
#include "lib/cd4051be.h"

void soil_humidity_isr()
{
    float sensor_val;
    cd4051be_setChannel(CH_2);
    sensor_val = cd4051be_ChannelRead(CH_2);
    Serial.printf("Analog Value = %.2f, ", sensor_val);
    sensor_val = (841-sensor_val)/4.05;
    Serial.printf("Sensor val = %.2f\n", sensor_val);
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
  
}

void loop()
{
  channel_e current_channel;
  
  current_channel = cd4051be_getCurrentChannel();

  soil_humidity_isr();

  delay(1000);
}
