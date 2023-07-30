add_library(stream_handler STATIC
${CMAKE_CURRENT_SOURCE_DIR}/in_order_output_stream_handler.cc
${CMAKE_CURRENT_SOURCE_DIR}/default_input_stream_handler.cc
${CMAKE_CURRENT_SOURCE_DIR}/default_input_stream_handler.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/immediate_input_stream_handler.cc
${CMAKE_CURRENT_SOURCE_DIR}/sync_set_input_stream_handler.cc
${CMAKE_CURRENT_SOURCE_DIR}/sync_set_input_stream_handler.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/early_close_input_stream_handler.cc
${CMAKE_CURRENT_SOURCE_DIR}/fixed_size_input_stream_handler.cc
${CMAKE_CURRENT_SOURCE_DIR}/fixed_size_input_stream_handler.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/barrier_input_stream_handler.cc
${CMAKE_CURRENT_SOURCE_DIR}/timestamp_align_input_stream_handler.cc
${CMAKE_CURRENT_SOURCE_DIR}/timestamp_align_input_stream_handler.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/mux_input_stream_handler.cc
)
target_link_libraries(stream_handler PUBLIC graph)
install(
  TARGETS
  stream_handler
  EXPORT
  ${PROJECT_NAME}Targets
  LIBRARY DESTINATION
  ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION
  ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION
  ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION
  include
  PUBLIC_HEADER DESTINATION
  include
)
FILE(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(
  FILES 
  ${HEADERS}
  DESTINATION
  ${MEDIAPIPE_INCLUDE_INSTALL_DIR}/framework/stream_handler
)
if(BUILD_TESTS)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/default_input_stream_handler_test.cc)
    set(src_list 
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc 
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    ) 
    set(dep framework)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/barrier_input_stream_handler_test.cc)
    set(src_list 
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    ) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/fixed_size_input_stream_handler_test.cc)
    set(src_list 
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/counting_source_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc
    ) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/immediate_input_stream_handler_test.cc)
    set(src_list 
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    ) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/mux_input_stream_handler_test.cc)
    set(src_list 
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/mux_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/gate_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/gate_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/make_pair_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/round_robin_demux_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    ) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/set_input_stream_handler_test.cc)
    set(src_list 
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/round_robin_demux_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/mux_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    ) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/sync_set_input_stream_handler_test.cc)
    set(src_list 
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/test_calculators.cc 
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/test_calculators.pb.cc 
    ) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/timestamp_align_input_stream_handler_test.cc
    )
    set(src_list 
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/packet_cloner_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/packet_cloner_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc
    ) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
endif()