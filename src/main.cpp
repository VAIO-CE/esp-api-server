#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <routes.h>
#include "ESPAsyncWebServer.h"
#include "Secret.h"

AsyncWebServer server(80);
static const String firmwareVersion = "v1.0.0";

void vTaskServerSetup(void *pv);

void setup()
{
  Serial.begin(115200);
  vTaskDelay(500 / portTICK_PERIOD_MS);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(Secret::AP_SSID, Secret::AP_PASS, 1, 0, 1, false);
  vTaskDelay(500 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(vTaskServerSetup, "API server running!", 2 * 1024, NULL, 1, NULL, 0);

  vTaskDelete(NULL);
}

void loop()
{
  // Empty, will be scheduled by FreeRTOS
}

void vTaskServerSetup(void *pv)
{
  // Begin mDNS service
  if (!MDNS.begin("vaio"))
  {
    Serial.println("Error setting up mDNS");
    while (1)
    {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }

  Serial.println("mDNS started!");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/mac", HTTP_GET, handleGetSavedMac);
  server.onRequestBody(handleOnBodyPost);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("API server running. Connect at http://vaio.local/");
  vTaskDelay(500 / portTICK_PERIOD_MS);

  vTaskDelete(NULL);
}