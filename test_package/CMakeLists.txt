cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(mediapipelite REQUIRED)
find_package(absl REQUIRED)
find_package(glog REQUIRED)
find_package(protobuf REQUIRED)
add_executable(example example.cc
in_order_output_stream_handler.cc
default_input_stream_handler.cc
default_input_stream_handler.pb.cc
)
target_link_libraries(example  
                      -Wl,--no-whole-archive 
mediapipelite::graph abseil::abseil glog::glog protobuf::protobuf)