#ifndef ROUTES_H
#define ROUTES_H

#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include "Preferences.h"

void handleRoot(AsyncWebServerRequest *req);
void handleReceiveMac(AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total);
void handleNotFound(AsyncWebServerRequest *req);

#endif