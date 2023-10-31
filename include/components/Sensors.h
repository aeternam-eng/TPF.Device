#pragma once

#include <Adafruit_AHTX0.h>

class SensorMeasurement {
 public:
  double temperature;
  double humidity;
};

class Sensors {
 private:
  static TwoWire i2cBus;
  static Adafruit_AHTX0 sensor;

 public:
  static void init() {
    i2cBus.begin(GPIO_NUM_14, GPIO_NUM_15, 100000);
    log_d("AHT10 startup status: %d", sensor.begin(&i2cBus));
  }

  static SensorMeasurement getData() {
    SensorMeasurement value;

    sensors_event_t temperature, humidity;

    if (!sensor.getEvent(&humidity, &temperature)) {
      log_e("AHT10 error status: %d", sensor.getStatus());

      value.humidity = 0;
      value.temperature = 0;

      return value;
    }

    value.temperature = temperature.temperature;
    value.humidity = humidity.relative_humidity;

    return value;
  }
};

Adafruit_AHTX0 Sensors::sensor;
TwoWire Sensors::i2cBus = TwoWire(0);
