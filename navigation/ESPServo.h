#ifndef espservo_h
#define espservo_h

#include "Arduino.h"

const int PWM_IN_FREQUENCY = 200;
const int DEFAULT_BIT_RESOLUTION = 0;

const int MIN_PWM_OUT = 55;
const int MAX_PWM_OUT = 100;

void setupServo(int pin, int channel, int freq=PWM_IN_FREQUENCY, int bits=DEFAULT_BIT_RESOLUTION) {
  ledcAttachPin(pin, channel);
  ledcSetup(channel, freq, bits);
}

#endif
