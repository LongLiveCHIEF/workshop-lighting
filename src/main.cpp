#include <WiFiManager.h>
#include <Arduino.h>
#include <Ticker.h>

WiFiManager wm;
Ticker blinker;

void blink(){
  int state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // blink every 0.3s
  blinker.attach(0.3, blink);

  // setup wifi AP
  WiFi.mode(WIFI_STA);

  Serial.begin(9600);

  wm.setConfigPortalBlocking(false);

  if(wm.autoConnect("espAP")){
    Serial.println("connected...");
  }
  else {
    Serial.println("Config portal running");
  }
}

void loop() {
  wm.process();

}
