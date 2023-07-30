add_executable(run_graph run_graph.cc
${PROJECT_SOURCE_DIR}/mediapipe/calculators/core/pass_through_calculator.cc
)
target_link_libraries(run_graph PUBLIC 
                          -Wl,--whole-archive 
                          stream_handler)
target_link_libraries(run_graph PUBLIC 
                          -Wl,--no-whole-archive 
                          graph
                    )
add_executable(new_node new_node.cc
)
target_link_libraries(new_node PUBLIC 
                          -Wl,--whole-archive 
                          stream_handler)
target_link_libraries(new_node PUBLIC 
                          -Wl,--no-whole-archive 
                          graph
                    )
add_executable(new_node_2 api2/new_node.cc
)
target_link_libraries(new_node_2 PUBLIC 
                          -Wl,--whole-archive 
                          stream_handler)
target_link_libraries(new_node_2 PUBLIC 
                          -Wl,--no-whole-archive 
                          graph
                    )
add_executable(cpp_graph api2/cpp_graph.cc
)
target_link_libraries(cpp_graph PUBLIC 
                          -Wl,--whole-archive 
                          stream_handler)
target_link_libraries(cpp_graph PUBLIC 
                          -Wl,--no-whole-archive 
                          graph
                    )