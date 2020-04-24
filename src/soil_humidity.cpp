
#include "../lib/soil_humidity.h"

float soil_humidity_get_vwc()
{
  float voltage_val;
  
  voltage_val = 100-((voltage_val-0.452)/0.00402);

  return voltage_val;
}
