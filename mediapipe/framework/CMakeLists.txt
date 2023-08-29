add_library(graph 

${CMAKE_CURRENT_SOURCE_DIR}/timestamp.cc
${CMAKE_CURRENT_SOURCE_DIR}/basic_types_registration.cc
${CMAKE_CURRENT_SOURCE_DIR}/calculator.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/calculator_base.cc
${CMAKE_CURRENT_SOURCE_DIR}/calculator_context.cc
${CMAKE_CURRENT_SOURCE_DIR}/calculator_context_manager.cc
${CMAKE_CURRENT_SOURCE_DIR}/calculator_contract.cc
${CMAKE_CURRENT_SOURCE_DIR}/calculator_graph.cc
${CMAKE_CURRENT_SOURCE_DIR}/calculator_node.cc
${CMAKE_CURRENT_SOURCE_DIR}/calculator_options.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/calculator_profile.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/calculator_runner.cc
${CMAKE_CURRENT_SOURCE_DIR}/calculator_state.cc
${CMAKE_CURRENT_SOURCE_DIR}/collection_item_id.cc
${CMAKE_CURRENT_SOURCE_DIR}/counter_factory.cc
${CMAKE_CURRENT_SOURCE_DIR}/delegating_executor.cc
${CMAKE_CURRENT_SOURCE_DIR}/executor.cc
${CMAKE_CURRENT_SOURCE_DIR}/graph_output_stream.cc
${CMAKE_CURRENT_SOURCE_DIR}/graph_service_manager.cc
${CMAKE_CURRENT_SOURCE_DIR}/input_side_packet_handler.cc
${CMAKE_CURRENT_SOURCE_DIR}/input_stream_handler.cc
${CMAKE_CURRENT_SOURCE_DIR}/input_stream_manager.cc
${CMAKE_CURRENT_SOURCE_DIR}/input_stream_shard.cc
${CMAKE_CURRENT_SOURCE_DIR}/legacy_calculator_support.cc
${CMAKE_CURRENT_SOURCE_DIR}/mediapipe_options.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/output_side_packet_impl.cc
${CMAKE_CURRENT_SOURCE_DIR}/output_stream_handler.cc
${CMAKE_CURRENT_SOURCE_DIR}/output_stream_manager.cc
${CMAKE_CURRENT_SOURCE_DIR}/output_stream_shard.cc
${CMAKE_CURRENT_SOURCE_DIR}/packet.cc
${CMAKE_CURRENT_SOURCE_DIR}/packet_type.cc
${CMAKE_CURRENT_SOURCE_DIR}/packet_factory.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/packet_generator.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/packet_generator_graph.cc
${CMAKE_CURRENT_SOURCE_DIR}/scheduler.cc
${CMAKE_CURRENT_SOURCE_DIR}/scheduler_queue.cc
${CMAKE_CURRENT_SOURCE_DIR}/status_handler.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/stream_handler.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/subgraph.cc
${CMAKE_CURRENT_SOURCE_DIR}/thread_pool_executor.cc
${CMAKE_CURRENT_SOURCE_DIR}/thread_pool_executor.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/timestamp.cc
${CMAKE_CURRENT_SOURCE_DIR}/validated_graph_config.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/tag_map.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/tag_map_helper.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/status_util.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/name_util.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/fill_packet_set.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/template_expander.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/proto_util_lite.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/validate_name.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/subgraph_expansion.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/options_util.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/options_syntax_util.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/options_field_util.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/options_registry.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/field_data.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/calculator_graph_template.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/packet_generator_wrapper_calculator.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/packet_generator_wrapper_calculator.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/simulation_clock.cc
${CMAKE_CURRENT_SOURCE_DIR}/tool/simulation_clock_executor.cc
${PROJECT_SOURCE_DIR}/mediapipe/util/cpu_util.cc
#/opt/android-ndk/sources/android/cpufeatures/cpu-features.c
)
add_subdirectory(api2)
add_subdirectory(deps)
if(ENABLE_PROFILER)
add_subdirectory(profiler)
endif()
target_link_libraries(graph PUBLIC abseil::abseil glog::glog protobuf::protobuf) 
add_subdirectory(stream_handler)
if(NOT BUILD_GRAPH_ONLY)
add_library(framework )
add_subdirectory(formats)
add_subdirectory(tool)
target_link_libraries(framework PUBLIC graph stb::stb opencv::opencv_core_alias opencv::opencv_highgui_alias opencv::opencv_imgproc_alias opencv::opencv_videoio_alias gpu util) 
endif()
install(
  TARGETS
  graph
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
  ${MEDIAPIPE_INCLUDE_INSTALL_DIR}/framework
)

FILE(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/port/*.h")
install(
  FILES 
  ${HEADERS}
  DESTINATION
  ${MEDIAPIPE_INCLUDE_INSTALL_DIR}/framework/port
)
FILE(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/tool/*.h")
install(
  FILES 
  ${HEADERS}
  DESTINATION
  ${MEDIAPIPE_INCLUDE_INSTALL_DIR}/framework/tool
)
FILE(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/profiler/*.h")
install(
  FILES 
  ${HEADERS}
  DESTINATION
  ${MEDIAPIPE_INCLUDE_INSTALL_DIR}/framework/profiler
)
if(BUILD_TESTS)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/calculator_node_test.cc)
    set(src_list 
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/tool/source.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/tool/source.pb.cc
    ) 
    set(dep framework)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/calculator_base_test.cc)
    set(src_list) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/calculator_context_test.cc)
    set(src_list ${CMAKE_CURRENT_SOURCE_DIR}/testdata/night_light_calculator.pb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/testdata/sky_light_calculator.pb.cc
    ) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/calculator_contract_test.cc)
    set(src_list 
    ${CMAKE_CURRENT_SOURCE_DIR}/calculator_contract_test.pb.cc
    ) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/calculator_graph_bounds_test.cc)
    set(src_list ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/test_calculators.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/test_calculators.pb.cc) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/calculator_graph_event_loop_test.cc)
    set(src_list ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    )
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    endif()
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/calculator_graph_side_packet_test.cc
    )
    set(src_list ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/counting_source_calculator.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/test_calculators.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/test_calculators.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/packet_generator_wrapper_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/packet_generator_wrapper_calculator.pb.cc
    )
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/calculator_graph_stopping_test.cc
    )
    set(src_list
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    )
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/calculator_graph_test.cc
    )
    set(src_list ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/test_calculators.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/test_calculators.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/mux_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/counting_source_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    )
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    endif()
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/calculator_parallel_execution_test.cc
    )
    set(src_list
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    )
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/calculator_runner_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/collection_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/executor_external_build_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)

    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/graph_service_manager_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)

    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/graph_service_test.cc)
    set(src_list ${CMAKE_CURRENT_SOURCE_DIR}/test_service.cc)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/graph_validation_test.cc)
    set(src_list ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/default_side_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/constant_side_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/constant_side_packet_calculator.pb.cc)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/input_stream_manager_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/output_stream_manager_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/packet_delete_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/packet_generator_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/packet_registration_test.cc)
    set(src_list 
    ${CMAKE_CURRENT_SOURCE_DIR}/packet_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/packet_test.pb.cc
    )
    ADD_GTEST_UNITTEST(${test_src} src_list dep)

    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/packet_test.cc)
    set(src_list 
    ${CMAKE_CURRENT_SOURCE_DIR}/packet_test.pb.cc
    )
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    endif()
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/timestamp_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/validated_graph_config_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)

endif()