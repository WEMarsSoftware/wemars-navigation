/*
   Kyle Inzunza
   NAVIGATION.ino
   Sketch for the navigation board to read from sensors
   and communicate with webpage
*/

#define DEBUG 1;

#include "ProcessOptions.h"
#include <Wire.h>
#include <Adafruit_LSM303.h>
#include "WeMarsCompass.h"
#include "NavWebsocket.h";
#include <TinyGPS++.h>
//#include "ESPServo.h"



//serial pins
#define RXD2 16 //TX2
#define TXD2 17 //RX2
#define LED 2 //onboard led



const int SERVO_PIN[] = {14, 15, 16}; //placeholder
const int SERVO_CHANNEL[] = {0, 1, 2};

bool led_status = false;

Adafruit_LSM303 lsm;
TinyGPSPlus gps;


int timer;
int servoTimer;

void setup() {

  //begin serials
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); //gps recommended baud rate 9600

  pinMode(LED, OUTPUT);

  startWiFi();
  startServer();

#ifdef SERVOCONTROL
  //loop through servos
  for (int a = 0; a < 3; a++) {
    setupServo(SERVO_PIN[a], SERVO_CHANNEL[a]);
  }
#endif

  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
    while (1);
  }


  timer = millis(); //start timer
#ifdef SERVOCONTROL
  servoTimer = millis();
  servoUpdateTimer = millis();
#endif

}

void loop() {

  //while there are values saved in the serial buffer
  while (Serial2.available() > 0) {
    //read and encode data from serial
    byte gpsData = Serial2.read();
    gps.encode(gpsData);
  }

#ifdef SERVOCONTROL
  //update every 50ms
  if (millis() - servoTimer > 50) {

    for (int a = 0; a < 2; a++) {
      int temp = map(cameraAngle[0], 0, 180, MIN_PWM_OUT, MAX_PWM_OUT);
      ledcWrite(SERVO_CHANNEL[a], temp);
    }

    //if data has not been recieved
    if (millis() - servoUpdateTimer > 50) {
      servoLeft = false;
      servoRight = false;
      ledcWrite(SERVO_CHANNEL[2], (MAX_PWM_OUT + MIN_PWM_OUT) / 2); //stop servo
    }
    else if (servoLeft) {
      ledcWrite(SERVO_CHANNEL[2], MAX_PWM_OUT);
    }
    else if (servoRight) {
      ledcWrite(SERVO_CHANNEL[2], MIN_PWM_OUT);
    }
    servoTimer = millis(); //reset timer
  }
#endif

  if (millis() - timer > 1000) {
    timer = millis(); //reset timer

    //blink LED
    if (led_status) {
      digitalWrite(LED, LOW);
    }
    else {
      digitalWrite(LED, HIGH);
    }
    led_status = !led_status;

    lsm.read();
    updateData();

#ifdef DEBUG
    Serial.println(generateSentence(getAngleX(), getAngleY(), getBearing(), gps.location.lat(), gps.location.lng()));
#endif
    writeServer(generateSentence(getAngleX(), getAngleY(), getBearing(), gps.location.lat(), gps.location.lng()));
  }
}

//generates string to send data over websocket
String generateSentence(float angleX, float angleY, float bearing, float latitude, float longitude) {
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
void updateData() {
  //populate data
  a_data[0] = (int)lsm.accelData.x;
  a_data[1] = (int)lsm.accelData.y;
  a_data[2] = (int)lsm.accelData.z;
  m_data[0] = (int)lsm.magData.x;
  m_data[1] = (int)lsm.magData.y;
  m_data[2] = (int)lsm.magData.z;
}
