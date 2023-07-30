target_sources(graph PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/gl_context_profiler.cc
${CMAKE_CURRENT_SOURCE_DIR}/graph_profiler.cc
${CMAKE_CURRENT_SOURCE_DIR}/graph_tracer.cc
${CMAKE_CURRENT_SOURCE_DIR}/profiler_resource_util.cc
${CMAKE_CURRENT_SOURCE_DIR}/profiler_resource_util_common.cc
${CMAKE_CURRENT_SOURCE_DIR}/trace_builder.cc
)
add_subdirectory(reporter)
if(BUILD_TESTS)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/circular_buffer_test.cc)
    set(src_list) 
    set(dep framework)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/graph_tracer_test.cc)
    set(src_list
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/flow_limiter_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/flow_limiter_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/round_robin_demux_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/immediate_mux_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/test_calculators.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/annotation_overlay_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/util/annotation_overlay_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/test_calculators.pb.cc) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/reporter_test.cc)
    set(src_list) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/sharded_map_test.cc)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
endif()