#include <Arduino.h>

#include "shared/Constants.h"

class CreateMeasurementRequest {
 public:
  uint8_t* fileContents;
  size_t fileSize;
  std::string deviceId;
  std::string temperature;
  std::string humidity;
};

std::vector<uint8_t> convertToFormData(const CreateMeasurementRequest& request) {
  auto imageContent = string_format(
      "%s\r\n%s\r\n%s\r\n\r\n",
      Constants::FormDataBoundary,
      string_format(Constants::ImageContentDisposition, "request").c_str(),
      Constants::ImageContentType);

  auto otherFields = string_format(
      "\r\n%s\r\n%s\r\n\r\n%s\r\n%s\r\n%s\r\n\r\n%s\r\n%s\r\n%s\r\n\r\n%s\r\n%s\r\n",
      Constants::FormDataBoundary,
      string_format(Constants::TextContentDisposition, "deviceId").c_str(),
      request.deviceId.c_str(),
      Constants::FormDataBoundary,
      string_format(Constants::TextContentDisposition, "temperature").c_str(),
      request.temperature.c_str(),
      Constants::FormDataBoundary,
      string_format(Constants::TextContentDisposition, "humidity").c_str(),
      request.humidity.c_str(),
      Constants::FormDataBoundaryClose);

  auto formData = std::vector<uint8_t>(std::begin(imageContent), std::end(imageContent));
  formData.insert(formData.end(), request.fileContents, request.fileContents + request.fileSize);
  formData.insert(formData.end(), otherFields.begin(), otherFields.end());
  // std::copy(request.fileContents, request.fileContents + request.fileSize, std::back_inserter(formData));
  // std::copy(std::begin(otherFields), std::end(otherFields), std::back_inserter(formData));

  return formData;
}
