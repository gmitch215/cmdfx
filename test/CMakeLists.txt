cmake_minimum_required(VERSION 3.16)

include(CTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

# Tests
function(add_test_executable folder name)
    set(TEST_NAME "cmdfx-test-${folder}-${name}")

    add_executable("${TEST_NAME}" "src/${folder}/${name}.c" "src/test.h")
    target_link_libraries("${TEST_NAME}" PRIVATE cmdfx)
    target_include_directories("${TEST_NAME}" PRIVATE "${PROJECT_SOURCE_DIR}/include")

    add_dependencies("${TEST_NAME}" cmdfx)
    add_test(NAME ${folder}/${name} COMMAND "${TEST_NAME}")

    if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options("${TEST_NAME}" PRIVATE --coverage -lm -w)
        target_link_options("${TEST_NAME}" PRIVATE --coverage)
    endif()
endfunction()

function(add_manual_test name)
    set(TEST_NAME "cmdfx-test-manual-${name}")

    add_executable("${TEST_NAME}" "src/manual/${name}.c" "src/test.h")
    target_link_libraries("${TEST_NAME}" PRIVATE cmdfx)
    target_include_directories("${TEST_NAME}" PRIVATE "${PROJECT_SOURCE_DIR}/include")

    add_dependencies("${TEST_NAME}" cmdfx)

    if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options("${TEST_NAME}" PRIVATE -lm -w)
    endif()
endfunction()

# Automatic Tests
file(GLOB_RECURSE AUTO_TESTS "src/common/*.c" "src/physics/*.c")

foreach(TEST_FILE ${AUTO_TESTS})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    get_filename_component(TEST_FOLDER ${TEST_FILE} DIRECTORY)
    get_filename_component(TEST_FOLDER ${TEST_FOLDER} NAME)

    add_test_executable(${TEST_FOLDER} ${TEST_NAME})
endforeach()

# Manual Tests
file(GLOB MANUAL_TESTS "src/manual/*.c")

foreach(TEST_FILE ${MANUAL_TESTS})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_manual_test(${TEST_NAME})
endforeach()