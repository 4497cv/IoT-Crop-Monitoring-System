#ifndef ATMEGA328P_H
#define ATMEGA328P_H

#include <Arduino.h>
/* Definitions */
#define LM35DZ_PIN      A0
#define xDelay          100
#define NSAMPLES        20

#define NSTATES         3

float LM35DZ_get_temperature_celsius(void);
static void  LM35DZ_print_temperature_celsius(void);
void  LM35DZ_get_samples(void);
static void  LM35DZ_print_samples(void);
static void  LM35DZ_calculate_standard_deviation(void);
float get_current_temperature_celcius(void);
void LM35DZ_init(void);

#endif
