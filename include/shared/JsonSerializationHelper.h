#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

class JsonSerializationHelper {
  template <typename OutputType, const size_t bufferSize>
  static OutputType deserializeFromString(String input) {
    DynamicJsonDocument json(bufferSize);

    DeserializationError err = deserializeJson(json, input);

    if (err) {
      log_e("Failed to deserialize json from string. Error: %s", err.c_str());
      return OutputType();
    }

    OutputType value = json.as<OutputType>();
    return value;
  }
};
