#pragma once

#include <AsyncJson.h>

#include "components/Camera.h"

class CameraController {
 public:
  static ArRequestHandlerFunction capture;
};

ArRequestHandlerFunction CameraController::capture = [](AsyncWebServerRequest* request) {
  log_d("Capturing image...");

  camera_fb_t* frameBuffer = Camera::capture();

  request->send("image/jpeg", frameBuffer->len, [frameBuffer](uint8_t* buffer, size_t maxLen, size_t index) -> size_t {
    for (size_t i = 0; i < maxLen; i++)
      buffer[i] = frameBuffer->buf[i + index];

    return maxLen;
  });

  Camera::clear();
};
