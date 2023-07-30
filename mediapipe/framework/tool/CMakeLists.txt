target_sources(framework PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/container_util.cc
${CMAKE_CURRENT_SOURCE_DIR}/executor_util.cc
${CMAKE_CURRENT_SOURCE_DIR}/node_chain_subgraph.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/source.cc
${CMAKE_CURRENT_SOURCE_DIR}/source.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/switch_container.cc
${CMAKE_CURRENT_SOURCE_DIR}/switch_container.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/switch_demux_calculator.cc
${CMAKE_CURRENT_SOURCE_DIR}/switch_mux_calculator.cc
${CMAKE_CURRENT_SOURCE_DIR}/template_parser.cc
${CMAKE_CURRENT_SOURCE_DIR}/validate.cc
${CMAKE_CURRENT_SOURCE_DIR}/validate_type.cc
${CMAKE_CURRENT_SOURCE_DIR}/test_util.cc
)