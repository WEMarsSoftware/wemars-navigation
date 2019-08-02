/*
 * Kyle Inzunza
 * WEMARSCOMPASS.h
 * Parse data from the Adafruit LSM303
 */

#ifndef wemarscompass
#define wemarscompass

#include "Arduino.h"
#include <Adafruit_LSM303.h>

int a_data[] = {0,0,0}; //accelerometer vector
int m_data[] = {0,0,0}; //magnetometer vector

int x_unit = {1,0,0}; //i vector
int y_unit = {0,1,0}; //j vector

float compres = 22.5; //resolution of compass directions


//updates vectors from board data
void updateData(lsm303AccelData accelerometer,lsm303MagData magnetometer){
    //populate data
    a_data[0] = (int)accelerometer.x;
    a_data[1] = (int)accelerometer.y;
    a_data[2] = (int)accelerometer.z;
    m_data[0] = (int)magnetometer.x;
    m_data[1] = (int)magnetometer.y;
    m_data[2] = (int)magnetometer.z; 
}

//returns angle of X axis to ground
float getAngleX(){
  return angleVector(x_unit,a_data);
}

float getAngleY(){
  return angleVector(y_unit,a_data);
}

//get compass bearing
float getBearing(){
  float x_proj[3]; //i projected onto gravity plane
  float m_proj[3]; //magnetometer vectore projected onto gravity plane
  
  //project vectors
  for(int c = 0; c < 2; c++){
    x_proj[c] = projectVector(x_unit,a_data,c);
    m_proj[c] = projectVector(m_data,a_data,c);
  }

  return angleVector(x_proj,y_proj);//return compass bearing in degrees 
}

String bearingToDirection(float b){
  String compass;

  //compass labels
  const String comp1[] = {"E","NEE","NE","NEE","N","NNE","NW","NWW","W"};
  const String comp2[] = {"E","SEE","SE","SSE","S","SWW","SW","SWW","W"};
  
  //upper quadrants
  if (magVect[1] > 0){
    //loop through options
    for (int c = 0; c < 9; c++){
      //if angle is within the range
      if (b > (compres*c - compres/2) && b < (compres*(c+1) - compres/2)){
        compass = comp1[c]; //set compass bearing
        break; //end loop
      }
    }
  }
  //lower quadrants
  else if (magVect[1] < 0){
    //loop through options
    for (int c = 0; c < 9; c++){
      //if angle is within the range
      if (ang > (compres*c - compres/2) && ang < (compres*(c+1) - compres/2)){
        compass = comp2[c]; //set compass bearing
        break; //end loop
      }
    }
  }
  //on y axis
  else if (magVect[0] == 0){
    if (magVect[1] > 0){
      compass = "N";
    }
    else{
      compass = "S";
    }
  }
  //on x axis
  else{
    if (magVect[0] > 0){
      compass = "E";
    }
    else{
      compass = "W";
    }
  }

  return compass;
}


#endif
