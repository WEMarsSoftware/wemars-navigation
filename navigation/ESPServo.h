#ifndef espservo_h
#define espservo_h

#include "Arduino.h"

const int PWM_IN_FREQUENCY = 200;
const int DEFAULT_BIT_RESOLUTION = 8;

const int MIN_PWM_OUT = 13;
const int MAX_PWM_OUT = 100;

void setupServo(int pin, int channel, int freq=PWM_IN_FREQUENCY, int bits=DEFAULT_BIT_RESOLUTION) {
  ledcAttachPin(pin, channel);
  ledcSetup(channel, freq, bits);
}

//negative percent speed to go in other direction
void servo360(int channel, int percentSpeed){
  ledcWrite(channel, map(percentSpeed, -100, 100, MIN_PWM_OUT, MAX_PWM_OUT));
}

#endif
