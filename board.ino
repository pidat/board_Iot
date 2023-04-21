#include "DHT.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <BH1750.h>
#include <Wire.h>

#define FIREBASE_HOST "https://iot-duydat-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "4H00VeCA53OLUhBOoyUwCUYifpXb33hrMltat0Lq"
#define WIFI_SSID "Tretraune"
#define WIFI_PASSWORD "Tretraune"
#define DHTPIN 3
#define FAN_PIN 7
#define LIGHT_PIN 8
#define WATER_PIN 18    
#define DHTTYPE DHT11   // DHT 11
#define D8 2 //SCL
#define D7 1 //SDA

DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter(0x23);
FirebaseData fb;
bool light_Status = false;  
bool fan_Status = false;
bool water_Status = false;


void setup() {
  Serial.begin(115200);
  Serial.println(F("Success"));
  delay(1000);
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(WATER_PIN, OUTPUT);
  dht.begin();
  Wire.begin(D7,D8); // SDA, SCL
  lightMeter.begin();
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);
  int humd = dht.readHumidity();
  // Read temperature
  int temp = dht.readTemperature();
  // Read lux
  int lux = lightMeter.readLightLevel();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(humd) || isnan(temp)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humd);
  Serial.print("%");
  Serial.print("--Temperature: ");
  Serial.print(temp);
  Serial.println("°C");
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  
  if(Firebase.RTDB.setInt(&fb,"/iots/Humid",humd)){
    Serial.println();  Serial.println(humd);
//    Serial.print("success"+fb.dataPath());
//    Serial.println("(" + fb.dataType() + ")");
   }else{
    Serial.println("Failed: " +fb.errorReason());
    }

  if(Firebase.RTDB.setInt(&fb,"/iots/Temp",temp)){
    Serial.println();  Serial.println(temp);
//    Serial.print("success"+fb.dataPath());
//    Serial.println("(" + fb.dataType() + ")");
   }else{
    Serial.println("Failed: " +fb.errorReason());
    }

   if(Firebase.RTDB.setInt(&fb,"/iots/Insen",lux)){
    Serial.println();  Serial.println(lux);
//    Serial.print("success"+fb.dataPath());
//    Serial.println("(" + fb.dataType() + ")");
   }else{
    Serial.println("Failed: " +fb.errorReason());
    }
// ---------------- LIGHT STATUS-----------------
   if(Firebase.RTDB.getBool(&fb,"/tools/light")){
    if(fb.dataType() == "boolean"){
      light_Status = fb.boolData();
      Serial.println("success"+fb.dataPath()+":"  + light_Status + "("+ fb.dataType() +")");
      digitalWrite(LIGHT_PIN, light_Status);
      }
    }else{
    Serial.println("Failed: " +fb.errorReason());
    }
// ---------------- FAN STATUS-----------------    
    if(Firebase.RTDB.getBool(&fb,"/tools/fan")){
    if(fb.dataType() == "boolean"){
      fan_Status = fb.boolData();
      Serial.println("success"+fb.dataPath()+":"  + fan_Status + "("+ fb.dataType() +")");
      digitalWrite(FAN_PIN, fan_Status);
      }
    }else{
    Serial.println("Failed: " +fb.errorReason());
    }
// ---------------- WATER STATUS-----------------    
    if(Firebase.RTDB.getBool(&fb,"/tools/water")){
    if(fb.dataType() == "boolean"){
      water_Status = fb.boolData();
      Serial.println("success"+fb.dataPath()+":"  + water_Status + "("+ fb.dataType() +")");
      digitalWrite(WATER_PIN, water_Status);
      }
    }else{
    Serial.println("Failed: " +fb.errorReason());
    }
}
