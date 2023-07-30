
#ifndef MEDIAPIPE_IMAGE_SEQUENCE_H
#define MEDIAPIPE_IMAGE_SEQUENCE_H

#include <string>
#include <vector>

class ImageSequence {
 public:
  struct frame {
    frame(const std::string& img_path) : img_path_(img_path){};

    const std::string img_path_;
  };

  ImageSequence(const std::string& img_dir_path);
  virtual ~ImageSequence() = default;

  std::vector<frame> get_frames() const;

 private:
  std::vector<std::string> img_file_paths_;
};

#endif  // MEDIAPIPE_IMAGE_READER