cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(mediapipelite REQUIRED)
find_package(absl REQUIRED)
find_package(glog REQUIRED)
find_package(protobuf REQUIRED)
add_executable(example example.cc
)
add_library(stream_handler_whole_archive INTERFACE)
target_link_libraries(stream_handler_whole_archive INTERFACE
    -Wl,--whole-archive mediapipelite::stream_handler -Wl,--no-whole-archive)
target_link_libraries(example  stream_handler_whole_archive mediapipelite::graph abseil::abseil glog::glog protobuf::protobuf)