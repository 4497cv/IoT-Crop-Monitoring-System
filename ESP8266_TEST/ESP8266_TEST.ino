/****************************************
 * Include Libraries
 ****************************************/
#include "lm35dz.h"
#include "cd4051be.h"

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
}

void loop()
{
  volatile float sensor_val;
  lm35dz_t sensor_config;
  sensor_config.op_mode = cd4051be;
  LM35DZ_init(sensor_config);

  sensor_val = LM35DZ_get_current_temperature(celcius);
  Serial.println(sensor_val);
  delay(500);
}
