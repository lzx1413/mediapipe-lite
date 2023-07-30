add_library(util )
#find_package(libyuv REQUIRED)
target_sources(util PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/resource_util_default.cc
annotation_renderer.cc
#audio_decoder.cc
#audio_decoder.pb.cc
color.pb.cc
header_util.cc
#image_frame_util.cc
#image_test_utils.cc
label_map.pb.cc
label_map_util.cc
rectangle_util.cc
render_data.pb.cc
resource_util.cc
resource_util_default.cc
#tensor_to_detection.cc
time_series_util.cc
${PROJECT_SOURCE_DIR}/mediapipe/framework/formats/time_series_header.pb.cc
)
target_link_libraries(util PUBLIC abseil::abseil glog::glog protobuf::protobuf opencv::opencv Eigen3::Eigen) 