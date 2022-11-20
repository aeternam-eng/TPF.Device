#include <Arduino.h>

#include "shared/Constants.h"

class CreateMeasurementRequest {
 public:
  uint8_t* fileContents;
  size_t fileSize;
  std::string deviceId;
};

size_t convertToFormData(uint8_t* buffer, const CreateMeasurementRequest& request) {
  auto content = string_format(
      "%s\r\n%s\r\n%s\r\n\r\n",
      Constants::FormDataBoundary,
      string_format(Constants::ImageContentDisposition, "request").c_str(),
      Constants::ImageContentType);
  auto secondPart = string_format(
      "\r\n%s\r\n%s\r\n\r\n%s\r\n%s\r\n",
      Constants::FormDataBoundary,
      string_format(Constants::TextContentDisposition, "deviceId").c_str(),
      request.deviceId.c_str(),
      Constants::FormDataBoundaryClose);

  memcpy((void*)&buffer[0], (void*)content.c_str(), content.size());
  int size = content.size();

  memcpy((void*)&buffer[size], (void*)request.fileContents, request.fileSize);
  size += request.fileSize;

  memcpy((void*)&buffer[size], (void*)secondPart.c_str(), secondPart.size());
  size += secondPart.size();

  return size;
}
