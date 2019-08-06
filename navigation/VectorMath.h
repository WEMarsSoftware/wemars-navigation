/*
 * Kyle Inzunza
 * VECTORMATH.h
 * Functions for computing operations with vectors
 */

#ifndef vectormath_h
#define vectormath_h

#include "Arduino.h"

//returns magnitude of vector
float magnitude(float vector[]){
  return sqrt(pow(vector[0],2) + pow(vector[1],2) + pow(vector[2],2));
}


//returns dot project of two vectors
float dotProduct(float vector1[], float vector2[]){
  return vector1[0]*vector2[0] + vector1[1]*vector2[1] + vector1[2]*vector2[2];
}

int dotProduct(int vector1[], int vector2[]){
  return vector1[0]*vector2[0] + vector1[1]*vector2[1] + vector1[2]*vector2[2];
}

//returns component 'index' of vector projected onto plane
float projectVector(float vector[], float plane[],int index){
    return vector[index] - (dotProduct(vector,plane)/pow(magnitude(plane),2))*plane[index]; //calculate magnitude of projected vector component a 
}

//returns angle between two vectors in degrees
float angleVector(float vector1[], float vector2[]){
  return  RAD_TO_DEG * acos(dotProduct(vector1,vector2)/(magnitude(vector1)*magnitude(vector2)));
}


#endif
