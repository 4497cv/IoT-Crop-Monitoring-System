/*
    \file:   cd4051be.cpp
    \brief:  This is source file for the API for the cd405be multiplexor
    \author: César Villarreal @4497cv
    \date:   01/04/2020
*/
#include "../lib/cd4051be.h"

static channel_e ch_value_g;

static channel_t cd4051be_channels_g[CH_NUM]=
{
  {{CH_1, CH_2,  CH_0}},
  {{CH_2, CH_0,  CH_1}},
  {{CH_0, CH_1,  CH_2}}
};

void cd4051be_init(void)
{
  pinMode(COM,        INPUT); //COM port
  pinMode(MUX_C,     OUTPUT); //Digital pin 0
  pinMode(MUX_B,     OUTPUT); //Digital pin 1
  pinMode(MUX_A,     OUTPUT); //Digital pin 2
  cd4051be_setChannel(CH_0);
  ch_value_g = CH_0;
}

void cd4051be_setChannel(channel_e channelNumber)
{
  switch(channelNumber)
  {
    case CH_0:
        digitalWrite(MUX_C, LOW);
        digitalWrite(MUX_B, LOW);
        digitalWrite(MUX_A, LOW);
    break;
    case CH_1:
        digitalWrite(MUX_C, LOW);
        digitalWrite(MUX_B, LOW);
        digitalWrite(MUX_A, HIGH);
    break;
    case CH_2:
        digitalWrite(MUX_C, LOW);
        digitalWrite(MUX_B, HIGH);
        digitalWrite(MUX_A, LOW);
    break;
    default:
        digitalWrite(MUX_C, LOW);
        digitalWrite(MUX_B, LOW);
        digitalWrite(MUX_A, LOW);
    break;
  }
}

float cd4051be_ChannelRead(channel_e channelNumber)
{
  float analogReadValue;
  cd4051be_setChannel(channelNumber);
  delay(500);
  analogReadValue = analogRead(COM);
  return analogReadValue;
}

channel_e cd4051be_getCurrentChannel(void)
{
  return ch_value_g;
}

void cd4051be_SwitchChannel(void)
{
  ch_value_g = cd4051be_channels_g[ch_value_g].next[0];
}
