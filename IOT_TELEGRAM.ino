template<class T> inline Print &operator<<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
}

#include <time.h>
#include <WiFiManager.h>
#include "CTBot.h"
#include <PZEM004Tv30.h>
CTBot miBot;
CTBotInlineKeyboard miTeclado;
#if defined(ESP32)
PZEM004Tv30 pzem(Serial2, 12, 14);
#else
PZEM004Tv30 pzem(Serial2);
#endif
int r[6] = { 4, 16, 17, 5, 18, 19 };
int LED = 21;
#include <EEPROM.h>
#define EEPROM_SIZE 120
#include "token.h"

String getPzEmData() {
  float voltage = pzem.voltage(); // Lee el voltaje
  float current = pzem.current(); // Lee la corriente
  float power = pzem.power(); // Lee el consumo de energía
  float energy = pzem.energy();
  if (isnan(voltage) || isnan(current) || isnan(power) || isnan(energy)) {
    return "Error al leer los datos del PZEM";
  }
  String data = "Voltaje: " + String(voltage) + "V, Corriente: " + String(current) + "A, Potencia: " + String(power) + "W, Consumo:" + String(energy) + "kWh";
  return data;
}

void setup() {
  WiFi.mode(WIFI_STA);
    WiFiManager wm;
    bool res;
    res = wm.autoConnect("Regleta_WiFi","12345678."); 
    if(!res) {
      Serial.println("Falló la conexión");
      // ESP.restart();
    } 
    else {    
      Serial.println("Conectado :)");
    }
  EEPROM.begin(EEPROM_SIZE);
  struct tm timeinfo1;
  getLocalTime(&timeinfo1);
  int hora_actual = timeinfo1.tm_hour;
  int dia_actual = timeinfo1.tm_mday;
  int hora_lectura = EEPROM.read(117);
  int dia_lectura = EEPROM.read(118);
  float prom = 0;
  float prom_dia = 0;
  if ((hora_actual != hora_lectura) || (dia_actual != dia_lectura))
  {
    for(int i = 0; i < 60; i++){
      prom = prom + EEPROM.read(i);
      EEPROM.write(i,0);
      EEPROM.commit();
    }
    prom = round(prom/60);
    EEPROM.write(60+hora_lectura,prom);
    EEPROM.commit();
    if (dia_actual != dia_lectura){
          for(int i = 60; i < 84; i++){
      prom_dia = prom_dia + EEPROM.read(i);
      EEPROM.write(i,0);
      EEPROM.commit();
    }
    prom_dia = round((prom_dia/24));
    EEPROM.write(85,prom_dia);
    EEPROM.commit();
    }
  }
  EEPROM.write(118,timeinfo1.tm_mday);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("Iniciando Bot de Telegram");

  for (int i = 0; i < 6; i++) {
    pinMode(r[i], OUTPUT);
    digitalWrite(r[i], HIGH);
  }
  /*miBot.wifiConnect(ssid, password);*/

  miBot.setTelegramToken(token);

  /*if (miBot.testConnection()) {
    Serial.println("\n Conectado");
  } else {
    Serial.println("\n Problemas Auxilio");
  }*/
  
  miTeclado.addButton("ONC1", "onc1", CTBotKeyboardButtonQuery);
  miTeclado.addButton("OFFC1", "offc1", CTBotKeyboardButtonQuery);
  miTeclado.addRow();
  miTeclado.addButton("ONC2", "onc2", CTBotKeyboardButtonQuery);
  miTeclado.addButton("OFFC2", "offc2", CTBotKeyboardButtonQuery);
  miTeclado.addRow();
  miTeclado.addButton("ONC3", "onc3", CTBotKeyboardButtonQuery);
  miTeclado.addButton("OFFC3", "offc3", CTBotKeyboardButtonQuery);
  miTeclado.addRow();
  miTeclado.addButton("ONC4", "onc4", CTBotKeyboardButtonQuery);
  miTeclado.addButton("OFFC4", "offc4", CTBotKeyboardButtonQuery);
  miTeclado.addRow();
  miTeclado.addButton("ONC5", "onc5", CTBotKeyboardButtonQuery);
  miTeclado.addButton("OFFC5", "offc5", CTBotKeyboardButtonQuery);
  miTeclado.addRow();
  miTeclado.addButton("ONC6", "onc6", CTBotKeyboardButtonQuery);
  miTeclado.addButton("OFFC6", "offc6", CTBotKeyboardButtonQuery);
  miTeclado.addRow();
  miTeclado.addButton("ON_ALL", "on_all", CTBotKeyboardButtonQuery);
  miTeclado.addButton("OFF_ALL", "off_all", CTBotKeyboardButtonQuery);
  miTeclado.addRow();
  miTeclado.addButton("CONSUMO", "consumo", CTBotKeyboardButtonQuery);
  miTeclado.addButton("CONSUMO_DIA", "consumo_dia", CTBotKeyboardButtonQuery);
  miTeclado.addRow();
}


void loop() {
  int ultima_hora;
  float kWh;
  float lectura;
  float lectura2;
  TBMessage msg;
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  digitalWrite(LED, HIGH);
  if (miBot.getNewMessage(msg)) {
    if (msg.messageType == CTBotMessageText) {
      if (msg.text.equalsIgnoreCase("/start")) {
        miBot.sendMessage(msg.sender.id, "Bienvenido " + msg.sender.firstName + ", prueba /start he intenta usar los botones🤖");
        miBot.sendMessage(msg.sender.id, "Conectores", miTeclado);
      } 
    } else if (msg.messageType == CTBotMessageQuery) {
      Serial << "Mensaje: " << msg.sender.firstName << " - " << msg.text << "\n";
      if (msg.callbackQueryData.equals("onc1")) {
        Serial.println("Encender Conector 1");
        digitalWrite(r[0], LOW);
        miBot.endQuery(msg.callbackQueryID, "Conector 1 Encendido", true);
      } else if (msg.callbackQueryData.equals("offc1")) {
        Serial.println("Apagar Conector 1");
        digitalWrite(r[0], HIGH);
        miBot.endQuery(msg.callbackQueryID, "Conector 1 Apagado");
      } else if (msg.callbackQueryData.equals("onc2")) {
        Serial.println("Encender Conector 2");
        digitalWrite(r[1], LOW);
        miBot.endQuery(msg.callbackQueryID, "Conector 2 Encendido", true);
      } else if (msg.callbackQueryData.equals("offc2")) {
        Serial.println("Apagar Conector 2");
        digitalWrite(r[1], HIGH);
        miBot.endQuery(msg.callbackQueryID, "Conector 2 Apagado");
      } else if (msg.callbackQueryData.equals("onc3")) {
        Serial.println("Encender Conector 3");
        digitalWrite(r[2], LOW);
        miBot.endQuery(msg.callbackQueryID, "Conector 3 Encendido", true);
      } else if (msg.callbackQueryData.equals("offc3")) {
        Serial.println("Apagar Conector 3");
        digitalWrite(r[2], HIGH);
        miBot.endQuery(msg.callbackQueryID, "Conector 3 Apagado");
      } else if (msg.callbackQueryData.equals("onc4")) {
        Serial.println("Encender Conector 4");
        digitalWrite(r[3], LOW);
        miBot.endQuery(msg.callbackQueryID, "Conector 4 Encendido", true);
      } else if (msg.callbackQueryData.equals("offc4")) {
        Serial.println("Apagar Conector 4");
        digitalWrite(r[3], HIGH);
        miBot.endQuery(msg.callbackQueryID, "Conector 4 Apagado");
      } else if (msg.callbackQueryData.equals("onc5")) {
        Serial.println("Encender Conector 5");
        digitalWrite(r[4], LOW);
        miBot.endQuery(msg.callbackQueryID, "Conector 5 Encendido", true);
      } else if (msg.callbackQueryData.equals("offc5")) {
        Serial.println("Apagar Conector 5");
        digitalWrite(r[4], HIGH);
        miBot.endQuery(msg.callbackQueryID, "Conector 5 Apagado");
      } else if (msg.callbackQueryData.equals("onc6")) {
        Serial.println("Encender Conector 6");
        digitalWrite(r[5], LOW);
        miBot.endQuery(msg.callbackQueryID, "Conector 6 Encendido", true);
      } else if (msg.callbackQueryData.equals("offc6")) {
        Serial.println("Apagar Conector 6");
        digitalWrite(r[5], HIGH);
        miBot.endQuery(msg.callbackQueryID, "Conector 6 Apagado");
      } else if (msg.callbackQueryData.equals("on_all")) {
        Serial.println("Encender todos los conectores");
        for(int i = 0; i < 6; i++){
        digitalWrite(r[i], LOW);
        }
        miBot.endQuery(msg.callbackQueryID, "Todos los conectores encendidos", true);
      } else if (msg.callbackQueryData.equals("off_all")) {
        Serial.println("Apagar todos los conectores");
        for(int i = 0; i < 6; i++){
        digitalWrite(r[i], HIGH);
        }
        miBot.endQuery(msg.callbackQueryID, "Todos los conectores apagados");
      }else if (msg.callbackQueryData.equals("consumo")) {
        String data = getPzEmData();
        miBot.sendMessage(msg.sender.id, "Datos medidos:\n" + data);
        miBot.endQuery(msg.callbackQueryID, "Datos enviados correctamente", true);
      }else if (msg.callbackQueryData.equals("consumo_dia")) {
        ultima_hora = round(EEPROM.read(60+timeinfo.tm_hour-1));
        lectura = 0;
        for(int i = 0; i < 60; i++){
          lectura = lectura + EEPROM.read(i);
        }
        EEPROM.write(60+timeinfo.tm_hour,round(lectura/60));
        lectura = lectura/60;
        for(int i = 60; i < 84; i++){
          lectura2 = lectura2 + EEPROM.read(i);
        }
        kWh = (lectura2)/1000;
        miBot.sendMessage(msg.sender.id, "Consumo en la última hora:\nWh:" + String(ultima_hora) + "Consumo en esta hora:\nWh: " + String(lectura) + "\n Consumo de hoy: \nkWh: " + String(kWh));
        miBot.endQuery(msg.callbackQueryID, "Datos enviados correctamente", true);
      }
       else {
        miBot.endQuery(msg.callbackQueryID, "Comando no reconocido", false);
      }
    }
  }
  delay(250);
  //Logica de las lecturas
  int segundos = timeinfo.tm_sec;
  int mins = timeinfo.tm_min;
  int hours = timeinfo.tm_hour;
  EEPROM.write(118,timeinfo.tm_mday);
  EEPROM.commit();
  EEPROM.write(117,hours);
  EEPROM.commit();
  if (segundos == 59){
    EEPROM.write(mins,round(pzem.power()));
      if (mins == 59){
        float consumo_min= 0;
        float consumo_hora = 0;
        for(int j = 0; j < 60 ; j++){
          consumo_min = EEPROM.read(j);
          consumo_hora = consumo_hora + consumo_min;
          EEPROM.write(j,0);
          EEPROM.commit();
        }
        consumo_hora = round(consumo_hora/60);
        EEPROM.write(60+hours,consumo_hora);
        EEPROM.commit();
        if (hours == 23){
          float prom_dia = 0;
          for(int i = 60; i < 84; i++){
            prom_dia = prom_dia + (EEPROM.read(i));
            EEPROM.write(i,0);
            EEPROM.commit();
          }
          prom_dia = round((prom_dia/24));
          EEPROM.write(84,prom_dia);
          EEPROM.commit();
        }
      }
  }
}
