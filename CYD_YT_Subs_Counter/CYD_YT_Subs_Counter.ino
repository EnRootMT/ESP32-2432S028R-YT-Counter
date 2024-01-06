#include <TFT_eSPI.h> 
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "DSEG14_Classic_Regular_70.h"
#include "YouTube.h"

TFT_eSPI tft = TFT_eSPI();

//Put your WiFi Credentials here
const char* ssid = "xxxxxx";
const char* password = "xxxxxxxxxxxxx";

//URL Youtube
String URLYT = "https://www.googleapis.com/youtube/v3/channels?";
String ApiKeyYT = "xxxxxxxxxxxxxxxx";
String IDchanYT = "xxxxxxxxxxxxxxxx";

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  
  tft.init();
  tft.setRotation(1); //This is the display in landscape
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  //image
  tft.pushImage(96,0,128,128,YouTube);
}

void loop() {
// wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    //Set HTTP Request Final URL with Location and API key information
    http.begin(URLYT + "part=statistics&id=" + IDchanYT + "&key=" + ApiKeyYT);

    // start connection and send HTTP Request
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {

      //Read Data as a JSON string
      String JSON_Data = http.getString();
      Serial.println(JSON_Data);

      //Retrieve some information from the JSON format
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);
      JsonObject obj = doc.as<JsonObject>();

      const float subscribers = obj["items"][0]["statistics"]["subscriberCount"].as<float>();

      tft.setFreeFont(&DSEG14_Classic_Regular_70);
      tft.fillRect(0, 120, 320, 120,TFT_BLACK);

      if(subscribers < 1000){ // If the subscriber count is below 1000
        
        String subs_display =  String(subscribers, 0);
        tft.drawCentreString(subs_display,160,150,1);
      }
        else if(subscribers < 10000){  // If the subscriber count is below 10000
          const float subs_k = subscribers/1000;
          String stringsubscribers =  String(subs_k, 2);
          String subs_display = (stringsubscribers+"K");
          tft.drawCentreString(subs_display,160,150,1);
        }
          else if(subscribers < 100000)  // If the subscriber count is below 100000
          {
            const float subs_k = subscribers/1000;
            String stringsubscribers =  String(subs_k, 1);
            String subs_display = (stringsubscribers+"K");
            tft.drawCentreString(subs_display,160,150,1);
          }
            else if(subscribers < 1000000){  // If the subscriber count is below 1M
              const float subs_k = subscribers/1000;
              String stringsubscribers =  String(subs_k, 0);
              String subs_display = (stringsubscribers+"K");
              tft.drawCentreString(subs_display,160,150,1);
            }
              else if(subscribers < 10000000) { // If the subscriber count is below 10M
                const float subs_100k = subscribers/1000000;
                String stringsubscribers =  String(subs_100k, 2);
                String subs_display = (stringsubscribers+"M");
                tft.drawCentreString(subs_display,160,150,1);;
              }
                else if(subscribers < 100000000) { // If the subscriber count is below 100M
                  const float subs_100k = subscribers/1000000;
                  String stringsubscribers =  String(subs_100k, 1);
                  String subs_display = (stringsubscribers+"M");
                  tft.drawCentreString(subs_display,160,150,1);
                }
                  else if(subscribers < 1000000000) { // If the subscriber count is below 1000M 
                    const float subs_100k = subscribers/1000000;
                    String stringsubscribers =  String(subs_100k, 0);
                    String subs_display = (stringsubscribers+"M");
                    tft.drawCentreString(subs_display,160,150,1);
                  }
                
    } 
    else {
      Serial.println("Error!");
     // lcd.clear();
      //lcd.print("Can't Get DATA!");
    }
  
    http.end();

  }

  delay(30000);

}
