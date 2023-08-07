#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define emergency_button 23
#define blue_led 15
#define red_led 2

const char *ssid = "Estufa";
const char *password = "Andrios2022";

HTTPClient http;
String payload = "";

String serverPath_shutdonw = "http://10.0.0.116/printer/emergency_stop";
String serverPath_restart = "http://10.0.0.116/printer/gcode/script?script=FIRMWARE_RESTART";

void setup()
{
  Serial.begin(115200);
  delay(1000);

  digitalWrite(red_led, LOW);
  digitalWrite(blue_led, LOW);

  pinMode(emergency_button, INPUT);
  pinMode(red_led, OUTPUT);
  pinMode(blue_led, OUTPUT);

  WiFi.begin(ssid, password);
  delay(1000);


}

void loop()
{
  
  if(WiFi.status() != WL_CONNECTED)digitalWrite(red_led, HIGH), digitalWrite(blue_led, LOW);
  if(WiFi.status() == WL_CONNECTED)digitalWrite(blue_led, HIGH);
  while (WiFi.status() != WL_CONNECTED)
  {
    switch (WiFi.status())
    {
    case WL_NO_SSID_AVAIL:
      Serial.println("[WiFi] SSID not found");
      break;
    case WL_CONNECT_FAILED:
      Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
      break;
    case WL_CONNECTION_LOST:
      Serial.println("[WiFi] Connection was lost");
      break;
    case WL_SCAN_COMPLETED:
      Serial.println("[WiFi] Scan is completed");
      break;
    case WL_DISCONNECTED:
      Serial.println("[WiFi] WiFi is disconnected");
      break;
    case WL_CONNECTED:
      Serial.println("[WiFi] WiFi is connected!");
      Serial.print("[WiFi] IP address: ");
      Serial.println(WiFi.localIP());
      break;
    default:
      Serial.print("[WiFi] WiFi Status: ");
      Serial.println(WiFi.status());
      break;
    }
    delay(100);
  }
  digitalWrite(red_led, LOW);
  digitalWrite(red_led, LOW);
  if (digitalRead(emergency_button) == LOW)
  {
    while (digitalRead(emergency_button) == LOW)
    {
      http.begin(serverPath_shutdonw);
      delay(50);
      int httpResponseCode = http.GET();
      delay(50);
      Serial.println(httpResponseCode);
      delay(50);
    }
    http.begin(serverPath_restart);
    while(http.GET() != 200){
      http.begin(serverPath_restart);
      delay(500);
    }
  }
  delay(1000);
}
