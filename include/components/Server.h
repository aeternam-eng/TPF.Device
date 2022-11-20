#pragma once

#include <ESPAsyncWebServer.h>

#include "api/controllers/CameraController.h"

class TPFServer {
 private:
  static AsyncWebServer server;

 public:
  static void init() {
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "*");

    server.on("/api/camera/capture", HTTP_GET, CameraController::capture);

    server.onNotFound([](AsyncWebServerRequest* request) {
      if (request->method() == HTTP_OPTIONS)
        request->send(200);
      else
        request->send(404, "text/plain", "Content not found");
    });

    // server.serveStatic("/", LITTLEFS, "/site/").setDefaultFile("index.html");

    server.begin();
  }
};

AsyncWebServer TPFServer::server = AsyncWebServer(80);
