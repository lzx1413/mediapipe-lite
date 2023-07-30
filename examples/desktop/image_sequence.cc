#include "image_sequence.h"
#include "mediapipe/framework/deps/file_helpers.h"

ImageSequence::ImageSequence(const std::string& img_dir_path) {
  mediapipe::file::MatchFileTypeInDirectory(
      img_dir_path, ".jpg", &img_file_paths_);
}

std::vector<ImageSequence::frame> ImageSequence::get_frames() const {
    std::vector<frame> frames;
    for (unsigned int i = 0; i < img_file_paths_.size(); ++i) {
        frames.emplace_back(frame{img_file_paths_.at(i)});
    }
    return frames;
}