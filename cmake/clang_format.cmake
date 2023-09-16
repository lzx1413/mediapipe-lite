function(add_clang_format_target target_name)
    find_program(${PROJECT_NAME}_CLANG_FORMAT_BINARY clang-format)
    get_target_property(TARGET_SOURCES ${target_name} SOURCES)
    add_custom_target(clang-format
            COMMAND ${${PROJECT_NAME}_CLANG_FORMAT_BINARY}
            -i ${TARGET_SOURCES}
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    message(STATUS "Format the project using the `clang-format` target (i.e: cmake --build build --target clang-format).\n")
endfunction()