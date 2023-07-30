target_sources(graph PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/reporter.cc
${CMAKE_CURRENT_SOURCE_DIR}/statistic.cc

)
FILE(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(
  FILES 
  ${HEADERS}
  DESTINATION
  ${MEDIAPIPE_INCLUDE_INSTALL_DIR}/framework/profiler/reporter
)