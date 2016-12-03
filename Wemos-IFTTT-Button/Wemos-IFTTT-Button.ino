// 03 dec. 2016 Ewald
// Do Anything button
//
// short press will connect to IFTTT and perform any action you configured there
// examples are:
// - send a push message to your smartphone
// - post a tweet
// - send an email, switch on your hue lights, ring your IoT doorbell, unlock your IoT lock
// - send the weatherforcast as an SMS
// and many, many, more....
//
// long press will activate an accesspoint so you can configure the Wifi credentials of the network you are on

#include <ESP8266WiFi.h>          // https://github.com/esp8266/Arduino

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          // https://github.com/tzapu/WiFiManager

#include <HttpClient.h>           // so we can talk to IFTTT

#define TRIGGER_PIN D3            // Wemos ButtonShield is connected to D3

const char* host        = "maker.ifttt.com";          // maker channel of IFTTT
const char* trigger     = "button";
const char* triggerKey  = "**********************";  // Fill this with your IFTTT Maker key
  
boolean val             = true;                      // to hold LED state
long longPressTime      = 250;

void setup() {                                       // Setup Stuff, this is run once
  Serial.begin(115200);                              // the Serial port Stuff is just for debugging
  Serial.println("\n Starting...");
  pinMode(TRIGGER_PIN, INPUT);                       // so we can read the switch
  pinMode(BUILTIN_LED, OUTPUT);                      // set onboard LED as output
  digitalWrite(BUILTIN_LED, HIGH);                   // turn LED off
  }

void longpress() {          // long button press

    flash(3);               // signal that we're going into AP mode to receive WiFi configuration
    
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    wifiManager.setTimeout(120);                  // timeout after 120 seconds

    //WITHOUT THIS THE AP DOES NOT SEEM TO WORK PROPERLY WITH SDK 1.5 , update to at least 1.5.1
    //WiFi.mode(WIFI_STA);

    //Here we start an access point with name "Do-Anything" and goe into a blocking loop awaiting configuration of timeout   
    if (!wifiManager.startConfigPortal("Do-Anything")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      Serial.println("resetting ESP....");
      ESP.reset();
      delay(5000);
      } //fi check

    //if you get here you have connected to the WiFi
    Serial.println("");
    Serial.print("Connected to: ");
    Serial.print(WiFi.SSID());
    Serial.print(" (RSSi: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm) ");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

  } //longpress

void flash(int y) {                 // flash LED y times
  for (int x=0; x < y; x++) {
    digitalWrite(BUILTIN_LED, LOW);
    delay(100);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(100);
    }
  } //flash
  
void shortpress() {                 // short button press
  Serial.println("Click");
  val = !val;
  digitalWrite(BUILTIN_LED, val);   // sets the LED to the button's value
  } //shortpress

void loop() {                                         // program loop, do untill doomsday 
  if ( digitalRead(TRIGGER_PIN) == LOW ) {            // button pressed
    delay(longPressTime);                             // wait for 250ms
    
    if ( digitalRead(TRIGGER_PIN) == LOW ) {          // button still pressed?
      longpress();
      }
    else {
      shortpress();
      }
    }

  } //loop
