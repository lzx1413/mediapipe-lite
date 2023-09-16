cmake_minimum_required(VERSION 3.15)
project(
mediapipelite 
VERSION 0.1.0
LANGUAGES CXX C)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()
if (CMAKE_BUILD_TYPE MATCHES "Release")
    # 开启空间优化
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")
endif()
option(BUILD_SHARED_LIBS "Build  as a shared library" OFF)
option(BUILD_PROTO_FILES "build protobuf files to cpp " ON)
option(BUILD_TESTS "build unit tests" ON)
option(USE_CCACHE "use ccache to accelerate build" OFF)
option(ENABLE_PROFILER "enable graph profiler buiold" OFF)
option(WITH_GPU "support gpu runtime" OFF)
option(BUILD_PYTHON "build  python bindings" OFF)
option(BUILD_GRAPH_ONLY "only build graph module" OFF)
option(BUILD_EXAMPLES "build  examples " ON)
option(ENABLE_RTTI "enable rtti " ON)
if (ANDROID)
include_directories($ENV{ANDROID_NDK_HOME}/sources/android)
add_definitions(-DMEDIAPIPE_MOBILE)
endif()
if(USE_CCACHE AND NOT MSVC)
    find_program(CCACHE_EXE ccache)
    if(CCACHE_EXE )
      set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_EXE}")
      set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_EXE}")
      message(STATUS "ccache: Enabled")
    endif()
else()
    set(USE_CCACHE OFF)
    message(STATUS "ccache: Disabled")
endif()
if(ENABLE_PROFILER)
    add_definitions(-DMEDIAPIPE_PROFILER_AVAILABLE=1)
endif()
if (NOT WITH_GPU)
    add_definitions(-DMEDIAPIPE_DISABLE_GPU=1)
endif()
if (NOT BUILD_PYTHON AND NOT BUILD_TESTS)
    if (NOT ENABLE_RTTI)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
    endif()
endif()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DNOMINMAX)
set(CMAKE_CXX_STANDARD 20)
endif()
include(${PROJECT_SOURCE_DIR}/cmake/clang_format.cmake)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "    C++ flags (Release):"    ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE})
message(STATUS "    C++ flags (Debug):"      ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG})
message(STATUS "    C Compiler:"             ${CMAKE_C_COMPILER} ${CMAKE_C_COMPILER_ARG1})
message(STATUS "    C++ Compiler:"             ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1})
message(STATUS "    C flags (Release):"      ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE})
message(STATUS "    C flags (Debug):"        ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
find_package(absl REQUIRED)
find_package(glog REQUIRED)
find_package(protobuf CONFIG REQUIRED)
if(NOT BUILD_GRAPH_ONLY)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(stb REQUIRED)
endif()
include_directories(${PROJECT_SOURCE_DIR})
# generate protobuf files
if(BUILD_PROTO_FILES)
    file(GLOB protobuf_files
            mediapipe/framework/*.proto
            mediapipe/framework/tool/*.proto
            mediapipe/framework/deps/*.proto
            mediapipe/framework/testdata/*.proto
            mediapipe/framework/formats/*.proto
            mediapipe/framework/formats/annotation/*.proto
            mediapipe/framework/formats/motion/*.proto
            mediapipe/framework/formats/object_detection/*.proto
            mediapipe/framework/stream_handler/*.proto
            mediapipe/gpu/*.proto
            mediapipe/calculators/internal/*.proto
            mediapipe/calculators/core/*.proto
            mediapipe/calculators/tflite/*.proto
            mediapipe/calculators/image/*.proto
            mediapipe/calculators/util/*.proto
            mediapipe/calculators/video/*.proto
            mediapipe/util/*.proto
            )
    SET(PROTO_META_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})
    LIST(APPEND PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR})
    FOREACH(FIL ${protobuf_files})

    GET_FILENAME_COMPONENT(FIL_WE ${FIL} NAME_WE)

        string(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FIL})
        string(REGEX REPLACE "(.+)\\${FILE_NAME}.*" "\\1" FILE_PATH ${FIL})

        string(REGEX MATCH "(/mediapipe/framework.*|/mediapipe/util.*|/mediapipe/calculators/video/|/mediapipe/calculators/util/|/mediapipe/calculators/internal/|/mediapipe/calculators/tflite/|/mediapipe/calculators/image/|/mediapipe/gpu/|/mediapipe/calculators/core/|/mediapipe/util/)" OUT_PATH ${FILE_PATH})

        set(PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}${OUT_PATH}${FIL_WE}.pb.cc")
        set(PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}${OUT_PATH}${FIL_WE}.pb.h")

        EXECUTE_PROCESS(
                COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_META_BASE_DIR} ${FIL}
        )
        set_source_files_properties(${FILE_PATH}${FIL_WE}.pb.cc PROPERTIES GENERATED True)
        set_source_files_properties(${FILE_PATH}${FIL_WE}.pb.h PROPERTIES GENERATED True)
        # message("Copying " ${PROTO_SRCS} " to " ${FILE_PATH})

        file(COPY ${PROTO_SRCS} DESTINATION ${FILE_PATH})
        file(COPY ${PROTO_HDRS} DESTINATION ${FILE_PATH})

    ENDFOREACH()
endif()

include(GNUInstallDirs)

install(
  EXPORT
  ${PROJECT_NAME}Targets
  FILE
  ${PROJECT_NAME}Targets.cmake
  NAMESPACE
  ${PROJECT_NAME}::
  DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
#
# Add version header
#
configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/version.hpp.in
  include/${PROJECT_NAME_LOWERCASE}/version.hpp
  @ONLY
)

install(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/include/${PROJECT_NAME_LOWERCASE}/version.hpp
  DESTINATION
  include/${PROJECT_NAME_LOWERCASE}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${PROJECT_NAME}ConfigVersion.cmake
  VERSION
  ${PROJECT_VERSION}
  COMPATIBILITY
  SameMajorVersion
)

configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
  FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

#
# Generate export header if specified
#

if(${PROJECT_NAME}_GENERATE_EXPORT_HEADER)
  include(GenerateExportHeader)
  generate_export_header(${PROJECT_NAME})
  install(
    FILES
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME_LOWERCASE}_export.h
    DESTINATION
    include
  )

  message(STATUS "Generated the export header `${PROJECT_NAME_LOWERCASE}_export.h` and installed it.")
endif()

set(INCLUDES_DESTINATION include)
set(MEDIAPIPE_INCLUDE_INSTALL_DIR ${INCLUDES_DESTINATION}/mediapipe)

if(BUILD_TESTS)
enable_testing()
find_package(GTest REQUIRED)
include(${PROJECT_SOURCE_DIR}/cmake/macros.cmake)
endif()
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
add_subdirectory(mediapipe)