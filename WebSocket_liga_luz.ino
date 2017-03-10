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
#include <SocketIOClient.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>


#define evento "nome do evento"
#define uuid_dispositivo "testeLuz02"
#define debugar true
#define luz 15
#define off HIGH

StaticJsonBuffer<500> jsonBuffer;

SocketIOClient client;

char host[] = "saiot.ect.ufrn.br";
int port = 80;
char nsp[] = "dispositivo";

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
  WiFiManager wifis;
  wifis.autoConnect();
  //
  root["uuid"] = uuid_dispositivo;
  //  root["time"] = 1351824120;
  //  JsonArray& data = root.createNestedArray("data");
  //  data.add(double_with_n_digits(48.756080, 6));
  //  data.add(double_with_n_digits(2.302038, 6));
  root.printTo(JSON);
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  //

  if (!client.connect(host, port, nsp)) {
    Serial.println("connection failed");
    return;
  }
  if (client.connected())
  {
    Serial.println("\n\n\n\n\nConectou no servidor\n\n\n");
    client.send("/post/dispositivo/", JSON, "Connected !!!!");
  }
}


void atualiza_luz(int intensidade) {
  if (debugar) {
    Serial.println("Atualizando lampada");
  }
  if (intensidade < 5) {
    digitalWrite(luz, off);
  }
  if (intensidade >= 5 && intensidade < 256) {
    analogWrite(luz, intensidade);
  }
  else {
    Serial.println("deu merda");
    //respoder a porra
  }
}

void loop() {


  if (client.monitor()) {

    Serial.print("\n\n\nRID: ");
    Serial.println(RID);

    if (RID == evento) {
      //Serial.print("200\n\n Rcontent: ");
      Serial.println(Rcontent);
      Serial.print("Rname: ");
      Serial.println(Rname);
    }
    atualiza_luz(Rname.toInt());
  }
}
