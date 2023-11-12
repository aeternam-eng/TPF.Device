#pragma once

#include <Arduino.h>

#include "components/Camera.h"
#include "components/Sensors.h"
#include "models/CreateMeasurementRequest.h"
#include "shared/HttpHelper.h"

class MonitoringRoutine {
 private:
  static TaskHandle_t _monitoringTaskHandle;

  static void _monitoringTask(void* arg) {
    log_d("Starting monitoring task");

    for (;;) {
      if (WiFi.isConnected()) {
        auto sensorValues = Sensors::getData();

        log_i("Temp: %f | Humidity: %f", sensorValues.temperature, sensorValues.humidity);

        camera_fb_t* frameBuffer = Camera::capture();

        auto request = CreateMeasurementRequest();
        request.fileContents = frameBuffer->buf;
        request.fileSize = frameBuffer->len;
        request.deviceId = DEVICE_ID;
        request.temperature = string_format("%f", sensorValues.temperature);
        request.humidity = string_format("%f", sensorValues.humidity);

        HttpHelper::postFile<CreateMeasurementRequest>("/api/v1/fire", request);

        Camera::clear();
      } else {
        log_d("Tried to monitor with a picture, but device is not connected");
      }

      vTaskDelay(PICTURE_INTERVAL);
    }
  }

  static void _startMonitoringTask() {
    auto result = xTaskCreatePinnedToCore(
        _monitoringTask, "monitoring-task", 16767, NULL, 1, &_monitoringTaskHandle, tskNO_AFFINITY);

    log_d("result: %d", result);
  }

 public:
  static void init() { _startMonitoringTask(); }
};

TaskHandle_t MonitoringRoutine::_monitoringTaskHandle;
