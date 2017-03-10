#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <SocketIOClient.h>//#include <SocketIOsocket.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

#define evento "/post/dispositivo/"
#define uuid_dispositivo "lampada001"
#define debugar true
#define luz 5 //D2

StaticJsonBuffer<100> jsonBuffer;

SocketIOClient socket;

char host[] = "saiot.ect.ufrn.br";
int port = 80;

extern String RID;
extern String Rname;
extern String Rcontent;

unsigned long previousMillis = 0;
long interval = 5000;
unsigned long lastreply = 0;
unsigned long lastsend = 0;


String JSON;
JsonObject& root = jsonBuffer.createObject();

void light(String state) {
  int intensidade = state.toInt();
  if (intensidade == 0) {
    Serial.println("DESLIGOU");
    digitalWrite(luz, LOW);
  }
  else if (intensidade > 0 && intensidade < 256) {
    Serial.println("LIGOUUUUU");
    analogWrite(luz, intensidade);
  }
  else {
    Serial.println("TÁ UMA PORRA");
  }
}
void funcionaDois(String oQueChegou){
  Serial.println("funciona dois mesmo");
  Serial.println(oQueChegou);
  Serial.println("\n");
}

void setup() {
  pinMode(luz, OUTPUT);
  Serial.begin(115200);
  delay(10);

  WiFiManager wifis;
  wifis.autoConnect();

  root["uuid"] = uuid_dispositivo;
  root.printTo(JSON);

  if (!socket.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  if (socket.connected())
  {
    Serial.println("\n\n\n\n\nConectou no servidor\n\n\n");
    socket.emit("/post/dispositivo/", JSON);
  }
  socket.on("action", light);
  socket.on("action2", funcionaDois);

}// end Setup



void loop() {
  socket.monitor();
}


