#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <routes.h>
#include "ESPAsyncWebServer.h"
#include "Secret.h"

AsyncWebServer server(80);

void vTaskServerSetup(void *pv);
void vTaskServerRun(void *pv);

void setup()
{
  Serial.begin(115200);
  Serial.println("This is beginning!");
  vTaskDelay(500 / portTICK_PERIOD_MS);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(Secret::AP_SSID, Secret::AP_PASS, 1, 0, 1, false);

  Serial.println("WiFi AP mode started!");
  vTaskDelay(500 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(vTaskServerSetup, "API server created!", 2 * 1024, NULL, 1, NULL, 0);
  Serial.println("API finished setup");
  vTaskDelay(500 / portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(vTaskServerRun, "API server running!", 2 * 1024, NULL, 10, NULL, 1);
  Serial.println("All task created and running!");

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
  server.onRequestBody(handleReceiveMac);
  server.onNotFound(handleNotFound);

  vTaskDelete(NULL);
}

void vTaskServerRun(void *pv)
{
  while (1)
  {
    server.begin();
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}