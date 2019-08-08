/*
 * Kyle Inzunza
 * NAVIGATION.ino
 * Sketch for the navigation board to read from sensors
 * and communicate with webpage 
 */

#include <Wire.h>
#include <Adafruit_LSM303.h>
#include "WeMarsCompass.h"
#include "NavWebsocket.h";
#include <TinyGPS++.h>
#include <Servo.h>

//serial pins
#define RXD2 16 //TX2
#define TXD2 17 //RX2

Adafruit_LSM303 lsm;
TinyGPSPlus gps;
Servo servo0, servo1, servo2;

int timer;
int servoTimer; 

void setup() {

  //begin serials
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); //gps recommended baud rate 9600

  startWiFi();
  startServer();

  //TODO: attach pins
  servo0.attach();
  servo1.attach();
  servo2.attach();

  
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
    while(1);
  }


  timer = millis(); //start timer
  servoTimer = millis();
  servoUpdateTimer = millis();

}

void loop() {

  //while there are values saved in the serial buffer
  while(Serial2.available() > 0){ 
      //read and encode data from serial  
      byte gpsData = Serial2.read();
      gps.encode(gpsData);
  }

  //update every 50ms
  if(millis() - servoTimer > 50){
    
    servo0.write(cameraAngle[0]);
    servo1.write(cameraAngle[1]);
    
    //if data has not been recieved
    if (millis() - servoUpdateTimer > 50){
      servoLeft = false;
      servoRight = false;
      servo2.write(90); //turn off servo
    }
    else if(servoLeft){
      servo2.write(0); //fullspeed left
    }
    else if(servoRight){
      servo2.write(180); //fullspeed right
    }
    servoTimer = millis(); //reset timer
  }

  if(millis() - timer > 1000){
    timer = millis(); //reset timer

    lsm.read();
    updateData();
    writeServer(generateSentence(getAngleX(),getAngleY(),getBearing(),gps.location.lat(),gps.location.lng()));
  }
}

//generates string to send data over websocket
String generateSentence(float angleX, float angleY, float bearing, float latitude, float longitude){
  String temp = "";
  temp += angleX;
  temp += ",";
  temp += angleY;
  temp += ",";
  temp += bearing;
  temp += ",";
  temp += latitude;
  temp += ",";
  temp += longitude;
  return temp; 
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
