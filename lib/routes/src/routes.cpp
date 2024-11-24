#include <Arduino.h>
#include <WiFiClient.h>
#include "routes.h"

Preferences pref;
WiFiClient client;

// @GET("/")
void handleRoot(AsyncWebServerRequest *req)
{
  req->send(200, "text/plain", "Hello, World!");
}

// @POST("/mac")
void handleReceiveMac(AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total)
{
  if (req->url() == "/mac")
  {
    StaticJsonDocument<200> doc;
    DeserializationError err = deserializeJson(doc, data);

    if (err)
    {
      req->send(400, "application/json", "{\"message\":\"Invalid JSON format\"}");
      return;
    }

    const char *macAddress = doc["mac"];

    if (macAddress != nullptr)
    {
      pref.begin("DS4-NVS", false);
      Serial.println("Previously stored: " + pref.getString("btmac", "empty"));
      pref.putString("btmac", macAddress);

      String jsonResponse = "{\"message\":\"MAC address stored successfully\", \"mac\":\"" + pref.getString("btmac", "empty") + "\"}";

      req->send(200, "application/json", jsonResponse);
      pref.end();
    }
    else
    {
      req->send(400, "application/json", "{\"message\":\"MAC address not found in request\"}");
    }
  }
  else
  {
    req->send(400, "application/json", "{\"message\":\"No data received\"}");
  }
}

// 404 Callback
void handleNotFound(AsyncWebServerRequest *req)
{
  req->send(400, "application/json", "{\"message\":\"Not found\"}");
}