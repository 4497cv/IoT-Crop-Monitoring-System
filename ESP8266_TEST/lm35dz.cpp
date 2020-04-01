#ifndef LM35DZ_H
#define LM35DZ_H

#include <Arduino.h>
#include "lm35dz.h"



static float temperature_samples[NSAMPLES];
static unsigned int samples_counter = 0;
static float g_current_temp;

/* ~~~~~~~~~~ LM35DZ functions ~~~~~~~~~~  */

void LM35DZ_init(void)
{
  pinMode(LM35DZ_PIN,     INPUT);
}

/*  
    \brief      This function returns the actual temperature value in celsius
    \param[in]  void
    \param[out] float
*/  
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
  v_out = (data*3300)/1024;
  celcius_temp = v_out/10; /* 10 mv division*/
  return celcius_temp;
}

/*  
    \brief      This function prints the actual temperature value in celsius
    \param[in]  void
    \param[out] float
*/  
static void LM35DZ_print_temperature_celsius(void)
{
  float celcius_temp;
  celcius_temp = LM35DZ_get_temperature_celsius();
  Serial.print(LM35DZ_get_temperature_celsius());
  Serial.print(" C");
  Serial.print("\n\r");
}

/*  
    \brief    
    \param[in]  void
    \param[out] void
*/  
void LM35DZ_get_samples(void)
{
  int s_count;
  int s_flag = 0;
  
  s_count = 0;
  
  do
  {
    if(NSAMPLES == s_count)
    {
      //LM35DZ_print_samples();
      LM35DZ_calculate_standard_deviation();
      s_flag = 1;
    }
    else if(NSAMPLES > s_count)
    {
      /* CAPTURE TEMPERATURE VALUE */
      temperature_samples[s_count] = LM35DZ_get_temperature_celsius();
      s_count++;
    }
  }while(s_flag == 0);
}

/*  
    \brief      This function prints the samples array
    \param[in]  void
    \param[out] void
*/  
static void LM35DZ_print_samples(void)
{
  uint8_t i;
  for(i=0; i < NSAMPLES; i++)
  {
    Serial.print("i = ");
    Serial.print(i);
    Serial.print(" | ");
    Serial.print(temperature_samples[i]);
    Serial.print("\n\r");
  }
}

/*  
    \brief      This function calculates the standard deviation for the
                temperature and updates the current global temp. value
    \param[in]  void
    \param[out] void
*/  
static void LM35DZ_calculate_standard_deviation(void)
{
  float total_average;
  float sigma_sqrrt_val_av;
  float std_dev;
  uint8_t i;

  total_average = 0;
  sigma_sqrrt_val_av = 0;
  
  for(i=0; i < NSAMPLES; i++) total_average += temperature_samples[i];

  total_average = total_average/NSAMPLES;

  for(i=0; i < NSAMPLES; i++) sigma_sqrrt_val_av += sqrt(abs((temperature_samples[i] - total_average)));
  
  std_dev = sqrt(sigma_sqrrt_val_av/(NSAMPLES-1));
  
  if(std_dev < 1)
  {
     g_current_temp = total_average;
  }
}

float get_current_temperature_celcius(void)
{
   LM35DZ_get_samples();
   return g_current_temp;
}


#endif
