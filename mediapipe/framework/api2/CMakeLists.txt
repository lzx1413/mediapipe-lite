target_sources(graph
PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/node.cc
${CMAKE_CURRENT_SOURCE_DIR}/packet.cc
${CMAKE_CURRENT_SOURCE_DIR}/packet_nc.cc
)
FILE(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(
  FILES 
  ${HEADERS}
  DESTINATION
  ${MEDIAPIPE_INCLUDE_INSTALL_DIR}/framework/api2
)
if(BUILD_TESTS)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/node_test.cc)
    set(src_list
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.pb.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/calculators/internal/callback_packet_calculator.cc
    ${PROJECT_SOURCE_DIR}/mediapipe/framework/tool/sink.cc
    ) 
    set(dep framework)
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/contract_test.cc)
    set(src_list) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/packet2_test.cc)
    set(src_list) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/port_test.cc)
    set(src_list) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/subgraph_test.cc)
    set(src_list) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/tag_test.cc)
    set(src_list) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/tuple_test.cc)
    set(src_list) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
    set(test_src ${CMAKE_CURRENT_SOURCE_DIR}/builder_test.cc)
    set(src_list) 
    ADD_GTEST_UNITTEST(${test_src} src_list dep)
endif()