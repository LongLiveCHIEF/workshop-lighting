#include <WiFiManager.h>
WiFiManager wm;

void setup() {
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
