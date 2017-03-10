/*
   informações tipo api
   {"uuid":"dca4d0c86e8649829559d5e9ec26a2dc",
   "uuid_usuario_editor":"1","data_hora_criado"
   :"2017-02-06T08:52:02.000Z","data_hora_editado"
   :"2017-02-06T07:52:03.000Z",
   "nome":"Luz intensidade","descricao"
   :null,"status":1}]
*/
#include <ESP8266WiFi.h>
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
#define luz D4
#define off HIGH

void light(int );


StaticJsonBuffer<500> jsonBuffer;

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

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.flush();

  WiFiManager wifis;

  wifis.autoConnect();


  /*
     configuração para OTA
  */
  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  ArduinoOTA.setPassword((const char *)"sds");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");





  //
  root["uuid"] = uuid_dispositivo;
  //  root["time"] = 1351824120;
  //  JsonArray& data = root.createNestedArray("data");
  //  data.add(double_with_n_digits(48.756080, 6));
  //  data.add(double_with_n_digits(2.302038, 6));
  root.printTo(JSON);

  // We start by connecting to a WiFi network
  //

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

}// end Setup



void loop() {
  socket.monitor();

  ArduinoOTA.handle();
}




void light(String state) {
  int intensidade = state.toInt()-255;
  Serial.println("\n\n[STRING]\t" + state + "\n\n");
  Serial.println(intensidade);
  Serial.println("[light] " + state);

  if (intensidade < 5) {
    digitalWrite(luz, off);
  }
  else if (intensidade >= 5 && intensidade < 256) {
    analogWrite(luz, intensidade);
  }
  else {
    Serial.println("deu merda");
    //respoder a porra
  }
}


