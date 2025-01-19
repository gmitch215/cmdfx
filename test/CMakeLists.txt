cmake_minimum_required(VERSION 3.16)

include(CTest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

# Tests
function(add_test_executable folder name)
    add_executable(cmdfx-test-${folder}-${name} "src/${folder}/${name}.c" "src/test.h")
    target_link_libraries(cmdfx-test-${folder}-${name} PRIVATE cmdfx)
    target_include_directories(cmdfx-test-${folder}-${name} PRIVATE "${PROJECT_SOURCE_DIR}/include")
    target_include_directories(cmdfx-test-${folder}-${name} PRIVATE "${PROJECT_SOURCE_DIR}/include/cmdfx")

    add_dependencies(cmdfx-test-${folder}-${name} cmdfx)
    add_test(NAME ${folder}/${name} COMMAND cmdfx-test-${folder}-${name})
endfunction()

# Common
add_test_executable(common canvas_clear)
add_test_executable(common canvas_print)