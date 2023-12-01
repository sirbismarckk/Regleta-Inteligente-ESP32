template<class T> inline Print &operator<<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
}

#include "CTBot.h"
CTBot miBot;
CTBotInlineKeyboard miTeclado;

int r[6] = { 4, 16, 17, 5, 18, 19 };
int LED = 21;

#include "token.h"

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("Iniciando Bot de Telegram");

  for (int i = 0; i < 6; i++) {
    pinMode(r[i], OUTPUT);
    digitalWrite(r[i], HIGH);
  }
  miBot.wifiConnect(ssid, password);

  miBot.setTelegramToken(token);

  if (miBot.testConnection()) {
    Serial.println("\n Conectado");
  } else {
    Serial.println("\n Problemas Auxilio");
  }
  
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
}

void loop() {
  TBMessage msg;
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
      }
    }
  }
  delay(250);
}

/*void loop() {
  TBMessage msg;

  if (CTBotMessageText == miBot.getNewMessage(msg)) { //Verifica si hay un nuevo mensaje disponible
    Serial << "Mensaje: " << msg.sender.firstName << " - " <<  msg.text << "\n";

    String commandType = msg.text.substring(0, 2);  // Extract the command type ("ON" or "OFF")
    int connectorNum = msg.text.substring(3).toInt();  // Extract the connector number

    if (connectorNum >= 1 && connectorNum <= 6) {
      String action = (commandType.equalsIgnoreCase("ON") ? "Encender " : "Apagar ");
      int state = (commandType.equalsIgnoreCase("ON") ? HIGH : LOW);

      Serial.println(action + "Conector " + connectorNum);
      digitalWrite(r[connectorNum - 1], state);
      miBot.sendMessage(msg.sender.id, "Conector " + String(connectorNum) + (state == HIGH ? " Encendido" : " Apagado"));
    } else {
      miBot.sendMessage(msg.sender.id, "Bienvenido " + msg.sender.firstName + ",intenta usar: ON o OFF");
    }
  }
  delay(250);
}*/
