add_executable(simple_run_graph_main simple_run_graph_main.cc)
if (APPLE)
target_link_libraries(simple_run_graph_main PUBLIC
                          graph
                    )
target_link_libraries(simple_run_graph_main PUBLIC
                          -Wl,-force_load
                          stream_handler)
else()
target_link_libraries(simple_run_graph_main PUBLIC
                          -Wl,--whole-archive
                          stream_handler)
target_link_libraries(simple_run_graph_main PUBLIC
                      -Wl,--no-whole-archive
                          graph
                    )
endif()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
set_target_properties(simple_run_graph_main PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:stream_handler")
endif()