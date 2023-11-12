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

  template <typename T>
  static void postFile(const char* requestUri, T request) {
    _httpClient.begin(_wifiClient, _baseUrl, TPF_API_PORT, requestUri, false);
    _httpClient.addHeader(
        "Content-Type", "multipart/form-data; boundary=--------------------------321841453207809969838898");

    std::vector<uint8_t> buffer = convertToFormData(request);

    // std::vector<char> charBuffer = std::vector<char>(buffer.begin(), buffer.end());
    // std::string bufferString = std::string(charBuffer.begin(), charBuffer.end());

    // log_i("FormData: %d bytes;", bufferString.size());
    // for (int c = 0; c < bufferString.size(); c++) {
    //   printf("%c", bufferString[c]);
    // }
    // log_i("End FormData");

    int err = _httpClient.POST(buffer.data(), buffer.size());

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
