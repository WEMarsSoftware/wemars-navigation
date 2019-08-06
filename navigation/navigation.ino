/*
 * Kyle Inzunza
 * NAVIGATION.ino
 * Sketch for the navigation board to read from sensors
 * and communicate with webpage 
 */

#include <Wire.h>
#include <Adafruit_LSM303.h>
#include "WeMarsCompass.h"
#include <TinyGPS++.h>

//serial pins
#define RXD2 16 //TX2
#define TXD2 17 //RX2

Adafruit_LSM303 lsm;
TinyGPSPlus gps;

int timer;

void setup() {

  //begin serials
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); //gps recommended baud rate 9600

  
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
    while(1);
  }


  timer = millis(); //start timer

}

void loop() {

  //while there are values saved in the serial buffer
  while(Serial2.available() > 0){ 
      //read and encode data from serial  
      byte gpsData = Serial2.read();
      gps.encode(gpsData);
  }

  if(millis() - timer > 1000){
    timer = millis(); //reset timer

    lsm.read();
    updateData();
    Serial.println(getBearing());
  }
}

//generates string to send data over websocket
String generateSentence(float angleX, float angleY, float bearing, float latitude, float longitude){
  
}

//updates vectors from board data
void updateData(){
    //populate data
    a_data[0] = (int)lsm.accelData.x;
    a_data[1] = (int)lsm.accelData.y;
    a_data[2] = (int)lsm.accelData.z;
    m_data[0] = (int)lsm.magData.x;
    m_data[1] = (int)lsm.magData.y;
    m_data[2] = (int)lsm.magData.z; 
}
