#include <Arduino.h>

#include "components/Camera.h"
#include "components/Server.h"
#include "shared/HttpHelper.h"

#include "routines/MonitoringRoutine.h"

void setup() {
  Serial.begin(115200);
  log_i("Starting STRONZO TPF DEVICE - %s", GIT_REV);

  pinMode(GPIO_NUM_4, OUTPUT);
  pinMode(GPIO_NUM_14, INPUT);
  pinMode(GPIO_NUM_15, INPUT);
  digitalWrite(GPIO_NUM_4, LOW);

  WiFi.setSleep(false);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("STRONZO-TPF", "12345678");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.waitForConnectResult();

  HttpHelper::init();
  HttpHelper::setBaseUrl(TPF_API_URL);

  Sensors::init();
  Camera::init();

  TPFServer::init();

  MonitoringRoutine::init();
}

void loop() {
  vTaskDelay(10000);
  log_d("Heap: %d", esp_get_free_heap_size());
}
