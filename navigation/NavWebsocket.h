#ifndef navwebsocket_h
#define navwebsocket_h

#include "Arduino.h"
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

// INFO FOR LOCAL ROUTER
const char* ssid = "WE MARS Rover";
const char* password = "westill1";
//const char* ssid = "partybus";
//const char* password = "allaboard";

#ifdef SERVOCONTROL
bool servoRight = false; //if 360 servo should be moving right
bool servoLeft = false; //if 360 servo should be moving left
int servoUpdateTimer; //amount of time since 360 servo data recieved
int cameraAngle[] = {0, 0};
#endif

IPAddress staticIP(192, 168, 1, 101);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);




// COMMUNICATION CONSTANTS
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncWebSocketClient * globalClient = NULL; //client for server

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//if there is a websocket event
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {

  //if the websocket has connected
  if (type == WS_EVT_CONNECT) {
    Serial.println("Websocket client connection received");
    globalClient = client; //declare client
  }
  //if the websocket has disconnected
  else if (type == WS_EVT_DISCONNECT) {
    Serial.println("Client disconnected");
    globalClient = NULL; //to avoid errors
  }
  //if websocket has recieved data
  else if (type == WS_EVT_DATA) {

#ifdef DEBUG
    Serial.print(cameraAngle[0]); Serial.println(cameraAngle[1]);
#endif

#ifdef SERVOCONTROL

    //360 servo control
    if ((char)data[0] == 'l') {
      servoUpdateTimer = millis();
      servoLeft = true;
      servoRight = false;
    }
    else if ((char)data[0] == 'r') {
      servoUpdateTimer = millis();
      servoRight = true;
      servoLeft = false;
    }
    else {
      servoRight = false;
      servoLeft = false;

      String dataStr = "";
      for (int a = 0; a < len; a++) {
        //_ marks end of sentence
        if ((char)data[a] != '_') {
          dataStr += String((char)data[a]);
        }
        else {
          break;
        }
      }

      cameraAngle[0] = getValue(dataStr,',',0).toInt();
      cameraAngle[1] = getValue(dataStr,',',1).toInt();

    }


#endif

  }
}
//starts wifi
//must begin serial before calling this function
void startWiFi()
{


  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.config(staticIP, gateway, subnet);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#ifdef DEBUG
    Serial.print(".");
#endif
  }
#ifdef DEBUG
  Serial.println();
  Serial.println("CONNECTED TO " + String(ssid));
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
#endif
}



//starts server
void inline startServer() {
  //need this to write to client
  if (!SPIFFS.begin(true)) {
#ifdef DEBUG
    Serial.println("An Error has occurred while mounting SPIFFS");
#endif
    return;
  }

  //start server
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.on("/html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/ws.html", "text/html");
  });
  server.begin();
}

//writes to websocket
void writeServer(String message) {
  //if server is connected
  if (globalClient) {
    globalClient->text(message);
  }
}

#endif
