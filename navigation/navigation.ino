/*
 * Kyle Inzunza
 * NAVIGATION.ino
 * Sketch for the navigation board to read from sensors
 * and communicate with webpage 
 */

#include <Wire.h>
#include <Adafruit_LSM303.h>

Adafruit_LSM303 lsm;

int timer;

void setup() {

  Serial.begin(115200);
  
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
    while(1);
  }


  timer = millis(); //start timer

}

void loop() {
  

  if(millis() - timer > 1000){
    timer = millis(); //reset timer

    lsm.read();
    
    Serial.print("Accel X: "); Serial.print((int)lsm.accelData.x); Serial.print(" ");
    Serial.print("Y: "); Serial.print((int)lsm.accelData.y);       Serial.print(" ");
    Serial.print("Z: "); Serial.println((int)lsm.accelData.z);     Serial.print(" ");
    Serial.print("Mag X: "); Serial.print((int)lsm.magData.x);     Serial.print(" ");
    Serial.print("Y: "); Serial.print((int)lsm.magData.y);         Serial.print(" ");
    Serial.print("Z: "); Serial.println((int)lsm.magData.z);       Serial.print(" ");

  }
}
