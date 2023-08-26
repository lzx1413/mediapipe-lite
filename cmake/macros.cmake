function(ADD_GTEST_UNITTEST TEST_SRC SRC_LIST LIB)
    file(RELATIVE_PATH SOURCE_REL_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${TEST_SRC})
    # Test module name: test_foo_bar
    string(REGEX REPLACE "\\.cc$" "" TEST_MODULE_NAME ${SOURCE_REL_PATH})
    string(REPLACE "." "_" TEST_MODULE_NAME ${TEST_MODULE_NAME})
    string(REPLACE "/" "_" TEST_MODULE_NAME ${TEST_MODULE_NAME})
    # Executable name: test_foo_bar
    set(TEST_EXECUTABLE_NAME ${TEST_MODULE_NAME})

    # Create test executable
    add_executable(${TEST_EXECUTABLE_NAME} ${TEST_SRC} ${${SRC_LIST}})
    target_include_directories(${TEST_EXECUTABLE_NAME} SYSTEM
                               PRIVATE
                               ${PROJECT_SOURCE_DIR})
    target_link_libraries(${TEST_EXECUTABLE_NAME}
                          PUBLIC
                          -Wl,--whole-archive
                          stream_handler
                          )
    target_link_libraries(${TEST_EXECUTABLE_NAME}
                          PUBLIC
                          -Wl,--no-whole-archive
                          ${${LIB}}
                          GTest::gtest
                          GTest::gmock
                          GTest::gtest_main)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:stream_handler")
    endif()
    set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES
                          RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/test
                          RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/test
                          RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${PROJECT_BINARY_DIR}/test
                          RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${PROJECT_BINARY_DIR}/test)

    # Add test
    add_test(${TEST_MODULE_NAME} ${PROJECT_BINARY_DIR}/test/${TEST_EXECUTABLE_NAME})
endfunction()