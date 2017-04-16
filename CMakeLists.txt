cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(WITH_TESTS "also build tests" OFF)

# Projects
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

if(WITH_TESTS)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)
endif(WITH_TESTS)

add_custom_target(
    doc
    doxygen
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc/
)
