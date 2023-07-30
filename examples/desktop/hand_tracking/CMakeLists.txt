add_library(desktop_offline_calculators STATIC 
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/flow_limiter_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/gate_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/immediate_mux_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/packet_inner_join_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/previous_loopback_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/video/opencv_video_decoder_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/video/opencv_video_encoder_calculator.cc
)
add_library(hand_render_cpu STATIC
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/begin_loop_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/end_loop_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/gate_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/split_vector_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/annotation_overlay_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/collection_has_min_size_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/detections_to_render_data_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/labels_to_render_data_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/landmarks_to_render_data_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/rect_to_render_data_calculator.cc
)
add_library(hand_landmark_tracking_cpu

)
add_library(desktop_tflite_calculators STATIC 
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/constant_side_packet_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/merge_calculator.cc
)
target_link_libraries(desktop_offline_calculators PUBLIC desktop_offline_calculators hand_render_cpu hand_landmark_tracking_cpu)

add_executable(hand_tracking_cpu 
PRIVATE 
${PROJECT_SOURCE_DIR}/mediapipe/examples/desktop/demo_run_graph_main.cc
${PROJECT_SOURCE_DIR}/mediapipe/examples/desktop/image_sequence.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/begin_loop_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/end_loop_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/gate_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/split_vector_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/annotation_overlay_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/collection_has_min_size_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/detections_to_render_data_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/labels_to_render_data_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/landmarks_to_render_data_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/rect_to_render_data_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/tflite/tflite_model_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/local_file_contents_calculator.cc
${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/switch_container.cc
)