#pragma once

#include <Arduino.h>

#include "components/Camera.h"
#include "models/CreateMeasurementRequest.h"
#include "shared/HttpHelper.h"

class MonitoringRoutine {
 private:
  static TaskHandle_t _monitoringTaskHandle;

  static void _monitoringTask(void* arg) {
    for (;;) {
      if (WiFi.isConnected()) {
        camera_fb_t* frameBuffer = Camera::capture();

        auto request = CreateMeasurementRequest();
        request.fileContents = frameBuffer->buf;
        request.fileSize = frameBuffer->len;
        request.deviceId = DEVICE_ID;

        HttpHelper::postFile<CreateMeasurementRequest, 30000>("/api/v1/fire", request);

        Camera::clear();
      } else {
        log_d("Tried to monitor with a picture, but device is not connected");
      }

      vTaskDelay(PICTURE_INTERVAL);
    }
  }

  static void _startMonitoringTask() {
    xTaskCreatePinnedToCore(_monitoringTask, "monitoring-task", 50000, NULL, 1, &_monitoringTaskHandle, tskNO_AFFINITY);
  }

 public:
  static void init() { _startMonitoringTask(); }
};

TaskHandle_t MonitoringRoutine::_monitoringTaskHandle;
