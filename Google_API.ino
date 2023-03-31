#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
const char* wifiName = "VIMALRAJ";
const char* wifiPass = "vimot4263";
const char *host_1 = "https://maps.googleapis.com/maps/api/place/nearbysearch/json?location=27.2046%2C77.4977&radius=10000&type=hospital,medical&keyword=hospital&key=AIzaSyDFXOnblgz9iXzSQtsIsxwCWtVNMOCPRmI";
const char *host_2 = "https://maps.googleapis.com/maps/api/place/details/json?placeid=ChIJ02wsjiYhqTsRANPFIdaY-lo&key=AIzaSyDFXOnblgz9iXzSQtsIsxwCWtVNMOCPRmI";
void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifiName);
  WiFi.begin(wifiName, wifiPass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{
  HTTPClient http;
  Serial.print("Request Link:");
  Serial.println(host_1);
  http.begin(host_1);
  int httpCode = http.GET();
  String payload_1 = http.getString();
  Serial.print("Response Code:");
  Serial.println(httpCode);
  Serial.print("Returned data from Server:");
  Serial.println(payload_1);
  
  if(httpCode == 200)
  {
    place();
    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
    DynamicJsonDocument capacity(1024);
    JsonObject root = JsonBuffer.parseObject(payload_2);    
    Serial.println(F("Response:"));
    Serial.print(root["name"].as<char*>());
    Serial.println(root["formatted_phone_number"].as<char*>());
    Serial.println(root["data"][0].as<char*>());
    Serial.println(root["data"][1].as<char*>());
  }
  else
  {
    Serial.println("Error in response");
  }
  http.end();
  delay(5000);
}
void place()
{
  Serial.print("Request Link:");
  Serial.println(host_2);
  http.begin(host_2);
  int httpCode = http.GET();
  String payload_2 = http.getString();
  Serial.print("Response Code:");
  Serial.println(httpCode);
}
