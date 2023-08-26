add_executable(object_detection 
${PROJECT_SOURCE_DIR}/examples/desktop/demo_run_graph_main.cc
${PROJECT_SOURCE_DIR}/examples/desktop/image_sequence.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/concatenate_vector_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/flow_limiter_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/previous_loopback_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/split_vector_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/image/image_transformation_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/tflite/ssd_anchors_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/tflite/tflite_converter_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/tflite/tflite_inference_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/tflite/tflite_tensors_to_detections_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/annotation_overlay_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/detection_label_id_to_text_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/detections_to_render_data_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/non_max_suppression_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/util/render_data.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/video/opencv_video_decoder_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/video/opencv_video_encoder_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/concatenate_vector_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/detection_label_id_to_text_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/detections_to_render_data_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/non_max_suppression_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/video/opencv_video_encoder_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/flow_limiter_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/image/image_transformation_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/annotation_overlay_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/util/label_map.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/split_vector_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/tflite/tflite_converter_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/tflite/tflite_inference_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/tflite/tflite_tensors_to_detections_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/util/color.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/util/annotation_renderer.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/tflite/ssd_anchors_calculator.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/util/tflite/tflite_model_loader.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/image/rotation_mode.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/util/resource_util.cc
${PROJECT_SOURCE_DIR}/mediapipe/gpu/scale_mode.pb.cc
${PROJECT_SOURCE_DIR}/mediapipe/util/header_util.cc
)
find_package(tensorflowlite REQUIRED)
target_link_libraries(object_detection PUBLIC 
                          -Wl,--whole-archive 
                          stream_handler)
target_link_libraries(object_detection PUBLIC 
                          -Wl,--no-whole-archive 
framework tensorflow::tensorflowlite)
set_target_properties(object_detection PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:stream_handler")

add_executable(test_opencv ${CMAKE_CURRENT_SOURCE_DIR}/test_opencv.cc)
target_link_libraries(test_opencv PUBLIC opencv::opencv_videoio_alias opencv::opencv_core_alias)