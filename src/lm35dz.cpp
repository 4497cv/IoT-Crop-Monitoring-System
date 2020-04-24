/*
    \file:   lm35dz.cpp
    \brief:  This is source file for the API for the LM35DZ temperature sensor
    \author: César Villarreal @4497cv
    \date:   01/04/2020
*/

#include <Arduino.h>
#include "../lib/lm35dz.h"

static float H_LPF[7]={-0.0956, 0.0468, 0.3148, 0.4525, 0.3148, 0.0468, -0.0956};
static float xn[7];

static lm35dz_t sensor_config_g;

/*  
    \brief      This function initialices the LM35DZ temperature sensor
    \param[in]  lm35dz_t
    \param[out] float
*/  
void LM35DZ_init(lm35dz_t sensor_config)
{
  uint8_t i;
  sensor_config.analog_pin = A0;
  
  switch(sensor_config.op_mode)
  {
   case normal:
      sensor_config.analog_pin = sensor_config.analog_pin;
   break;
   case cd4051be:
      sensor_config.analog_pin = A0;
   break;
   default:
      sensor_config.analog_pin = A0;
   break;
  }
  
  pinMode(sensor_config.analog_pin,  INPUT);

  Serial.println("calibrating temperature sensor....");
  
  /* tomar muestras iniciales */
  for(i = 6; i >= 1; i--)
  {
    xn[i] = analogRead(sensor_config_g.analog_pin);
    delay(1000);
  }
}

/*  
    \brief      This function filters the temperature read value from the sensor
                to reduce noise.
    \param[in]  temp_e
    \param[out] float
*/  
float LM35DZ_get_current_temperature(temp_e temperature_format)
{
  uint8_t i;
  volatile float x0;
  volatile float y;
  
  /* inicializar el valor de la salida */
  y=0;
  /* lectura de valor del ADC  */
  x0 = LM35DZ_read_temperature_celsius();

  if(farenheit == temperature_format)
  {
    x0 = LM35DZ_temperature_C2F(x0);
  }

  /* tomar muestras iniciales */
  for(i = 6; i >= 1; i--) xn[i] = xn[i-1];
  /* guardar el valor de temperatura actual */
  xn[0] = x0;
  /* realizar convolución de la entrada con la respuesta h1/h2 */
  for(i = 0; i < 7 ; i++) y += xn[i]* H_LPF[i];
  
  return y;
}

/*  
    \brief      This function retrieves the current temperature value from the sensor in deg. celcius
    \param[in]  void
    \param[out] float
*/    
static float LM35DZ_read_temperature_celsius(void)
{
  volatile float data;
  float celcius_temp;
  float v_out;
  
  /* read analog data from temperature sensor */
  data = analogRead(sensor_config_g.analog_pin);
  /* The analog values read from the Arduino may have a value between 0 and 1024, 
   *  in which 0 corresponds to 0V and 1024 to 5V. So, we can easily get the output voltage of the sensor in mV */
  v_out = (data*3300)/1024;
  celcius_temp = v_out/10; /* 10 mv division*/
  return celcius_temp;
}

/*  
    \brief    
    \param[in]  void
    \param[out] float
*/  
static float LM35DZ_temperature_C2F(float temp_value)
{
  return temp_value = (temp_value*1.8)+32;
}
