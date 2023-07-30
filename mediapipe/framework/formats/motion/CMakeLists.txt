target_sources(framework PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/optical_flow_field_data.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/optical_flow_field.cc
)
FILE(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(
  FILES 
  ${HEADERS}
  DESTINATION
  ${MEDIAPIPE_INCLUDE_INSTALL_DIR}/framework/formats/motion
)