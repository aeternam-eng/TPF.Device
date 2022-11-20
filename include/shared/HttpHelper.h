#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include "shared/StringUtils.h"

class HttpHelper {
 private:
  static WiFiClientSecure _wifiClient;
  static HTTPClient _httpClient;
  static String _baseUrl;

 public:
  static void setBaseUrl(const char* baseUrl) { _baseUrl = baseUrl; }

  static void init() { _wifiClient.setInsecure(); }

  static void setUserAgent(const char* newUserAgent) { _httpClient.setUserAgent(newUserAgent); }

  template <typename T, const size_t bufferSize>
  static void postFile(const char* requestUri, T request) {
    _httpClient.begin(_wifiClient, _baseUrl, TPF_API_PORT, requestUri, false);
    _httpClient.addHeader(
        "Content-Type", "multipart/form-data; boundary=--------------------------306409604379268013737648");

    uint8_t buffer[bufferSize];
    int bufi = convertToFormData(buffer, request);

    int err = _httpClient.POST(buffer, bufi);

    if (err > 0) {
      log_d("Succesfully sent file POST to %s. Response: %s", requestUri, _httpClient.getString().c_str());
    } else {
      log_d("Error sending POST: %d | Error: %s", err, _httpClient.errorToString(err).c_str());
    }

    _httpClient.end();
  }
};

WiFiClientSecure HttpHelper::_wifiClient;
HTTPClient HttpHelper::_httpClient;
String HttpHelper::_baseUrl = "";
