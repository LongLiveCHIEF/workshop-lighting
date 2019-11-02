#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <Ticker.h>
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>

WiFiManager wm;

// initialize blinker to use as feedback during firmware updates
bool blinkerState;
void blink();
Ticker firmware_update_blinker(blink, 100);

// FastLED pre-processor definitions
#define NUM_LEDS 8*32
#define DATA_PIN 4
#define LED_TYPE WS2812B
#define RGB_ORDER GRB

CRGBArray<NUM_LEDS> leds;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  // setup LED's
  FastLED.addLeds<LED_TYPE, DATA_PIN, RGB_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
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
  //OTA
  ArduinoOTA.onStart([]() {
    firmware_update_blinker.start();
  });
  ArduinoOTA.onEnd([]() {
    firmware_update_blinker.stop();
    digitalWrite(LED_BUILTIN, false);
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    firmware_update_blinker.update();
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
}

void loop() {
  ArduinoOTA.handle();
  leds(0,NUM_LEDS - 1).fill_solid(CRGB::Snow);
}

void blink(){
  digitalWrite(LED_BUILTIN, blinkerState);
  blinkerState = !blinkerState;
}
