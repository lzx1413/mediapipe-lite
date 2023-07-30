add_library(gpu )
target_sources(gpu PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/gpu_service.cc
#${CMAKE_CURRENT_SOURCE_DIR}/gpu_shared_data_internal.cc
${CMAKE_CURRENT_SOURCE_DIR}/gpu_buffer_storage.cc
#${CMAKE_CURRENT_SOURCE_DIR}/gl_texture_buffer.cc
#${CMAKE_CURRENT_SOURCE_DIR}/gl_context_egl.cc
#${CMAKE_CURRENT_SOURCE_DIR}/gl_context.cc
${CMAKE_CURRENT_SOURCE_DIR}/gl_context_options.pb.cc
${CMAKE_CURRENT_SOURCE_DIR}/gpu_buffer.cc
${CMAKE_CURRENT_SOURCE_DIR}/gpu_buffer_format.cc
)
target_link_libraries(gpu PUBLIC abseil::abseil glog::glog protobuf::protobuf) 