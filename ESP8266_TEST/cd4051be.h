/*
    \file:   cd4051be.cpp
    \brief:  This is header file for the API for the cd405be multiplexor
    \author: César Villarreal @4497cv
    \date:   01/04/2020
*/

#ifndef CD4051BE_H
#define CD4051BE_H

#include <Arduino.h>

/* Definitions */
#define MUX_C          D2
#define MUX_B          D1
#define MUX_A          D0
#define COM            A0
#define CH_NUM         3

typedef enum
{
  CH_0,
  CH_1,
  CH_2
} channel_e;

typedef struct
{
  //void(*fptr)(void);  //Text string
  channel_e next[CH_NUM]; //FSM terminal states
} channel_t;

void cd4051be_init(void);
void cd4051be_setChannel(channel_e channelNumber);
float cd4051be_ChannelRead(channel_e channelNumber);
channel_e cd4051be_getCurrentChannel(void);
void cd4051be_SwitchChannel(void);


#endif
