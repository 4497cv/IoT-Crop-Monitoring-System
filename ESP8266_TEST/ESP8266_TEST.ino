/****************************************
 * Include Libraries
 ****************************************/
#include "lm35dz.h"
#include "cd4051be.h"
static float prev;
/****************************************
 * Main Functions
 ****************************************/
void setup()
{
  /* multiplexer module initialization */
  cd4051be_init();
  /* establish serial communication */
  Serial.begin(115200);
  cd4051be_setChannel(CH_0);
  prev = 0;
}

void loop()
{
  volatile float vout,temp;
  float m;
  lm35dz_t sensor_config;
  sensor_config.op_mode = cd4051be;
  LM35DZ_init(sensor_config);
  m = 10; // 10 mV
  vout = analogRead(LM35DZ_PIN)*(3300/1024);
  temp = vout/m;
  //Serial.printf("vout = %.2f, value = %i\n", vout, analogRead(LM35DZ_PIN));
  Serial.println(temp);
}
