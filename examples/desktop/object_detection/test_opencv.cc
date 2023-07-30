
#include <iostream>
#include <string>

#include "mediapipe/framework/port/opencv_highgui_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"

int main() {
  std::string video_path =
      "/mediapipe-lite/mediapipe/examples/desktop/object_detection/"
      "test_video.mp4";
  cv::VideoCapture capture;
  std::cout << video_path << std::endl;
  capture.open(video_path);
  std::cout << capture.isOpened() << std::endl;
  std::string image_path =
      "/mediapipe-lite/mediapipe/examples/desktop/object_detection/test.jpg";
  auto image = cv::imread(image_path);
  cv::imwrite("test1.jpg", image);
  return 0;
}