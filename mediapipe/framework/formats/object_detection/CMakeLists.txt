target_sources(framework PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/anchor.pb.cc
)
FILE(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(
  FILES 
  ${HEADERS}
  DESTINATION
  ${MEDIAPIPE_INCLUDE_INSTALL_DIR}/framework/formats/object_detection
)