class Constants {
 public:
  static constexpr const char* FormDataBoundary = "----------------------------306409604379268013737648";
  static constexpr const char* ImageContentDisposition =
      "Content-Disposition: form-data; name=\"%s\"; filename=\"img.jpg\"";
  static constexpr const char* ImageContentType = "Content-Type: image/jpeg";
  static constexpr const char* TextContentDisposition = "Content-Disposition: form-data; name=\"%s\";";
  static constexpr const char* FormDataBoundaryClose = "----------------------------306409604379268013737648--";
};
