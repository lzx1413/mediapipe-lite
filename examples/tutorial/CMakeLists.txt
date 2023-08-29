add_executable(run_graph run_graph.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc
)
if (APPLE)
target_link_libraries(run_graph PUBLIC 
                          graph
                    )
target_link_libraries(run_graph PUBLIC 
                          -Wl,-force_load
                          stream_handler)
else()
target_link_libraries(run_graph PUBLIC 
                          -Wl,--whole-archive 
                          stream_handler)
target_link_libraries(run_graph PUBLIC 
                      -Wl,--no-whole-archive 
                          graph
                    )
endif()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
set_target_properties(run_graph PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:stream_handler")
endif()
add_executable(new_node new_node.cc
)
if (APPLE)
target_link_libraries(new_node PUBLIC 
                      graph
                    )
target_link_libraries(new_node PUBLIC 
                          -Wl,-force_load
                          stream_handler)
else()
target_link_libraries(new_node PUBLIC 
                          -Wl,--whole-archive 
                          stream_handler)
target_link_libraries(new_node PUBLIC 
                      -Wl,--no-whole-archive 
                      graph
                    )
endif()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
set_target_properties(new_node PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:stream_handler")
endif()
add_executable(new_node_2 api2/new_node.cc
)
if (APPLE)
target_link_libraries(new_node_2 PUBLIC 
                          graph
                    )
target_link_libraries(new_node_2 PUBLIC 
                          -Wl,-force_load
                          stream_handler)
else()
target_link_libraries(new_node_2 PUBLIC 
                          -Wl,--whole-archive 
                          stream_handler)
target_link_libraries(new_node_2 PUBLIC 
                      -Wl,--no-whole-archive 
                          graph
                    )
endif()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
set_target_properties(new_node_2 PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:stream_handler")
endif()
add_executable(cpp_graph api2/cpp_graph.cc
)
if (APPLE)
target_link_libraries(cpp_graph PUBLIC 
                          graph
                    )
target_link_libraries(cpp_graph PUBLIC 
                          -Wl,-force_load
                          stream_handler)
else()
target_link_libraries(cpp_graph PUBLIC 
                          -Wl,--whole-archive 
                          stream_handler)
target_link_libraries(cpp_graph PUBLIC 
                      -Wl,--no-whole-archive 
                          graph
                    )
endif()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
set_target_properties(cpp_graph PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:stream_handler")
endif()