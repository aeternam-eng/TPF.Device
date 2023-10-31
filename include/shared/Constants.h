class Constants {
 public:
  static constexpr const char* FormDataBoundary = "----------------------------321841453207809969838898";
  static constexpr const char* ImageContentDisposition =
      "Content-Disposition: form-data; name=\"%s\"; filename=\"img.jpg\"";
  static constexpr const char* ImageContentType = "Content-Type: image/jpeg";
  static constexpr const char* TextContentDisposition = "Content-Disposition: form-data; name=\"%s\";";
  static constexpr const char* FormDataBoundaryClose = "----------------------------321841453207809969838898--";
};
