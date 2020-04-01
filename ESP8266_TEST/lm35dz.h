/*
    \file:   lm35dz.cpp
    \brief:  This is header file for the API for the LM35DZ temperature sensor
    \author: César Villarreal @4497cv
    \date:   01/04/2020
*/

#ifndef ATMEGA328P_H
#define ATMEGA328P_H

#include <Arduino.h>
/* Definitions */
#define LM35DZ_PIN      A0

typedef enum
{
  celcius,
  farenheit
} temp_e;

typedef enum
{
  normal,
  cd4051be
} lm35dz_op_mode_t;

typedef struct
{
  lm35dz_op_mode_t op_mode;
  int analog_pin;
} lm35dz_t;

/*  
    \brief      This function initialices the LM35DZ temperature sensor
    \param[in]  lm35dz_t
    \param[out] float
*/  
void LM35DZ_init(lm35dz_t sensor_config);

/*  
    \brief      This function filters the temperature read value from the sensor
                to reduce noise.
    \param[in]  temp_e
    \param[out] float
*/  
float LM35DZ_get_current_temperature(temp_e temperature_format);

/*  
    \brief      This function retrieves the current temperature value from the sensor in deg. celcius
    \param[in]  void
    \param[out] float
*/  
static float LM35DZ_read_temperature_celsius(void);

#endif
