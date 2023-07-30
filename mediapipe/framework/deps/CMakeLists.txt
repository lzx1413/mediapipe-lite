target_sources(graph
PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/clock.cc
${CMAKE_CURRENT_SOURCE_DIR}/file_helpers.cc
${CMAKE_CURRENT_SOURCE_DIR}/file_path.cc
${CMAKE_CURRENT_SOURCE_DIR}/monotonic_clock.cc
${CMAKE_CURRENT_SOURCE_DIR}/proto_descriptor.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/registration.cc
${CMAKE_CURRENT_SOURCE_DIR}/registration_token.cc
${CMAKE_CURRENT_SOURCE_DIR}/ret_check.cc
${CMAKE_CURRENT_SOURCE_DIR}/status.cc
${CMAKE_CURRENT_SOURCE_DIR}/status_builder.cc
#${CMAKE_CURRENT_SOURCE_DIR}/threadpool_pthread_impl.cc
${CMAKE_CURRENT_SOURCE_DIR}/threadpool_std_thread_impl.cc
${CMAKE_CURRENT_SOURCE_DIR}/topologicalsorter.cc
)
FILE(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(
  FILES 
  ${HEADERS}
  DESTINATION
  ${MEDIAPIPE_INCLUDE_INSTALL_DIR}/framework/deps
)
if(BUILD_TESTS)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/mathutil_unittest.cc)
    set(src_list)
    find_package(benchmark REQUIRED)
    set(dep framework benchmark::benchmark)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/monotonic_clock_test.cc)
    set(src_list)
    find_package(benchmark REQUIRED)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/registration_token_test.cc)
    set(src_list)
    set(dep framework)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/safe_int_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/status_builder_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/threadpool_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/topologicalsorter_test.cc)
    set(src_list)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
endif()