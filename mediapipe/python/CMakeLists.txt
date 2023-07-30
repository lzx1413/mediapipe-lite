set(PYTHON_VERSION "3.8" CACHE STRING "python version")
message(STATUS ${PYTHON_VERSION})
#find_package(Python ${PYTHON_VERSION} COMPONENTS Interpreter Development REQUIRED)
#set(PYBIND11_FINDPYTHON OFF)
find_package(pybind11 REQUIRED)
find_package(stb REQUIRED)
pybind11_add_module(_framework_bindings ${CMAKE_CURRENT_SOURCE_DIR}/pybind/matrix.cc
${CMAKE_CURRENT_SOURCE_DIR}/pybind/calculator_graph.cc
${CMAKE_CURRENT_SOURCE_DIR}/pybind/image.cc
${CMAKE_CURRENT_SOURCE_DIR}/pybind/image_frame.cc
${CMAKE_CURRENT_SOURCE_DIR}/pybind/packet.cc
${CMAKE_CURRENT_SOURCE_DIR}/pybind/packet_creator.cc
${CMAKE_CURRENT_SOURCE_DIR}/pybind/packet_getter.cc
${CMAKE_CURRENT_SOURCE_DIR}/pybind/resource_util.cc
${CMAKE_CURRENT_SOURCE_DIR}/pybind/timestamp.cc
${CMAKE_CURRENT_SOURCE_DIR}/pybind/validated_graph_config.cc
${CMAKE_CURRENT_SOURCE_DIR}/framework_bindings.cc
)
target_link_libraries(_framework_bindings PUBLIC stb::stb framework)
set_target_properties(_framework_bindings PROPERTIES  
LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
