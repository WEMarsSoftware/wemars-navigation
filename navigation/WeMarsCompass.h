/*
 * Kyle Inzunza
 * WEMARSCOMPASS.h
 * Parse data from the Adafruit LSM303
 */

#ifndef wemarscompass
#define wemarscompass

#include "Arduino.h"
#include "VectorMath.h"
#include <Adafruit_LSM303.h>

float a_data[] = {0,0,0}; //accelerometer vector
float m_data[] = {0,0,0}; //magnetometer vector

float x_unit[] = {1,0,0}; //i vector
float y_unit[] = {0,1,0}; //j vector


float m_proj[3]; //magnetometer vector projected onto gravity plane

float compres = 22.5; //resolution of compass directions

//returns angle of X axis to ground
float getAngleX(){
  return angleVector(x_unit,a_data) - 90;
}

float getAngleY(){
  return angleVector(y_unit,a_data) - 90;
}

//TODO: try using adafruit solution

//get compass bearing
float getBearing(){

  // Calculate the angle of the vector y,x
  float heading = (atan2(m_data[1],m_data[0]) * 180) / PI;
  
  // Normalize to 0-360
  if (heading < 0)
  {
    heading = 360 + heading;
  }
  return heading;
  
  /*
  float x_proj[3]; //i projected onto gravity plane
  
  //project vectors
  for(int c = 0; c < 2; c++){
    x_proj[c] = projectVector(x_unit,a_data,c);
    m_proj[c] = projectVector(m_data,a_data,c);
  }

  return angleVector(x_proj,m_proj);//return compass bearing in degrees 
  */

}


#endif
