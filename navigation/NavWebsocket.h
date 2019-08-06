#ifndef navwebsocket.h
#define navwebsocket.h

#include "Arduino.h"
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

// INFO FOR LOCAL ROUTER
const char* ssid = "WE MARS Rover";
const char* password = "westill1";

int cameraAngle[] = {0,0}


// COMMUNICATION CONSTANTS
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncWebSocketClient * globalClient = NULL; //client for server

//if there is a websocket event
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){

  //if the websocket has connected
  if(type == WS_EVT_CONNECT){
    Serial.println("Websocket client connection received");
    globalClient = client; //declare client
  }
  //if the websocket has disconnected
  else if(type == WS_EVT_DISCONNECT){
    Serial.println("Client disconnected");
    globalClient = NULL; //to avoid errors
  }
  //if websocket has recieved data
  else if(type == WS_EVT_DATA){
    String tempData = "";
    int cameraCounter = 0;
    int counter = 0; //how many values have been read
    //loop through sentence 
    for (int i = 0; i < len; i++){
      if((char)data[i] != ',' && counter < 3){
        counter++;
        String temp = String((char)data[i]); //convert data to string
        tempData += temp; 
      }
      //all needed values have been read
      else if(counter >= 3 && cameraCounter >= 2){
        break; //stop reading
      }
      //new value
      else{
        cameraAngle[counter] = tempData.toInt();
        cameraCounter++;
        //reset values
        counter = 0;
        tempData = "";
      }
    }
    
    
  }
}
 
//starts wifi
//must begin serial before calling this function
void inline startWiFi()
{  
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
    Serial.println("CONNECTED TO " + String(ssid));
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.macAddress());
}

//starts server
void inline startServer(){
  //need this to write to client
  if(!SPIFFS.begin(true)){
     Serial.println("An Error has occurred while mounting SPIFFS");
     return;
  }

  //start server
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.on("/html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/ws.html", "text/html");
  });
  server.begin();
}

//writes to websocket
void writeServer(String message){
  //if server is connected
  if (globalClient){
    globalClient->text(message);
  }
} 

#endif
