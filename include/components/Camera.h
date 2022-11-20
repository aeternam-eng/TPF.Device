#pragma once

#include <Arduino.h>
#include <esp_camera.h>
#include "camera_pins.h"

class Camera {
 private:
  static camera_config_t cameraConfig;
  static camera_fb_t* frameBufferPointer;

  static void buildCameraConfig() {
    cameraConfig.ledc_channel = LEDC_CHANNEL_0;
    cameraConfig.ledc_timer = LEDC_TIMER_0;
    cameraConfig.pin_d0 = Y2_GPIO_NUM;
    cameraConfig.pin_d1 = Y3_GPIO_NUM;
    cameraConfig.pin_d2 = Y4_GPIO_NUM;
    cameraConfig.pin_d3 = Y5_GPIO_NUM;
    cameraConfig.pin_d4 = Y6_GPIO_NUM;
    cameraConfig.pin_d5 = Y7_GPIO_NUM;
    cameraConfig.pin_d6 = Y8_GPIO_NUM;
    cameraConfig.pin_d7 = Y9_GPIO_NUM;
    cameraConfig.pin_xclk = XCLK_GPIO_NUM;
    cameraConfig.pin_pclk = PCLK_GPIO_NUM;
    cameraConfig.pin_vsync = VSYNC_GPIO_NUM;
    cameraConfig.pin_href = HREF_GPIO_NUM;
    cameraConfig.pin_sscb_sda = SIOD_GPIO_NUM;
    cameraConfig.pin_sscb_scl = SIOC_GPIO_NUM;
    cameraConfig.pin_pwdn = PWDN_GPIO_NUM;
    cameraConfig.pin_reset = RESET_GPIO_NUM;
    cameraConfig.xclk_freq_hz = 10000000;
    cameraConfig.frame_size = FRAMESIZE_VGA;
    cameraConfig.pixel_format = PIXFORMAT_JPEG;  // for streaming
    // cameraConfig.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
    cameraConfig.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    cameraConfig.fb_location = CAMERA_FB_IN_PSRAM;
    cameraConfig.jpeg_quality = 12;
    cameraConfig.fb_count = 1;

    // cameraConfig.pixel_format = PIXFORMAT_YUV422;

    if (cameraConfig.pixel_format == PIXFORMAT_JPEG) {
      if (psramFound()) {
        log_d("PSRAM found, using higher quality for JPG");

        cameraConfig.jpeg_quality = 16;
        cameraConfig.fb_count = 8;
        cameraConfig.grab_mode = CAMERA_GRAB_LATEST;
      } else {
        // Limit the frame size when PSRAM is not available
        cameraConfig.frame_size = FRAMESIZE_SVGA;
        cameraConfig.fb_location = CAMERA_FB_IN_DRAM;
      }
    } else {
      // Best option for face detection/recognition
      cameraConfig.frame_size = FRAMESIZE_240X240;
    }
  }

 public:
  static void init() {
    buildCameraConfig();

    esp_err_t error = esp_camera_init(&cameraConfig);

    if (error != ESP_OK) {
      log_e("Error initializing camera.");
    }

    sensor_t* s = esp_camera_sensor_get();
    s->set_brightness(s, 2);      // -2 to 2
    s->set_contrast(s, 2);        // -2 to 2
    s->set_saturation(s, -2);     // -2 to 2
    s->set_special_effect(s, 0);  // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5
                                  // - Blue Tint, 6 - Sepia)
    s->set_whitebal(s, 1);        // 0 = disable , 1 = enable
    s->set_awb_gain(s, 1);        // 0 = disable , 1 = enable
    s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
    s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
    s->set_aec2(s, 1);           // 0 = disable , 1 = enable
    s->set_ae_level(s, 1);       // -2 to 2
    s->set_aec_value(s, 500);    // 0 to 1200
    s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
    s->set_agc_gain(s, 20);      // 0 to 30
    s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
    s->set_bpc(s, 0);                         // 0 = disable , 1 = enable
    s->set_wpc(s, 1);                         // 0 = disable , 1 = enable
    s->set_raw_gma(s, 1);                     // 0 = disable , 1 = enable
    s->set_lenc(s, 0);                        // 0 = disable , 1 = enable
    s->set_hmirror(s, 0);                     // 0 = disable , 1 = enable
    s->set_vflip(s, 0);                       // 0 = disable , 1 = enable
    s->set_dcw(s, 0);                         // 0 = disable , 1 = enable
    s->set_colorbar(s, 0);                    // 0 = disable , 1 = enable
  }

  static camera_fb_t* capture() {
    // if flash is wanted, the flash has to be toggled ~150ms before getting the framebuffer
    frameBufferPointer = esp_camera_fb_get();

    log_i("Captured image: %u bytes", frameBufferPointer->len);

    return frameBufferPointer;
  }

  static void clear() { esp_camera_fb_return(frameBufferPointer); }
};

camera_config_t Camera::cameraConfig;
camera_fb_t* Camera::frameBufferPointer;
