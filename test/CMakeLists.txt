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

# Common
add_test_executable(common util_time)
add_test_executable(common util_math_clamp)
add_test_executable(common util_math_lerp)
add_test_executable(common util_color_lerp)

add_test_executable(common screen)

add_test_executable(common canvas_clear)
add_test_executable(common canvas_dimensions)

## Physics
add_test_executable(physics engine)
add_test_executable(physics mass)

# Manual
add_manual_test(events_resize)
add_manual_test(events_key)
add_manual_test(events_mouse)

add_manual_test(canvas_print)
add_manual_test(canvas_animated)

add_manual_test(sprites_move)
add_manual_test(sprites_resize)
add_manual_test(sprites_ansi)