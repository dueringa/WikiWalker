cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(WW_WITH_TESTS "also build tests" OFF)

enable_testing()

# Projects
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

if(WW_WITH_TESTS)
    add_subdirectory(${CMAKE_SOURCE_DIR}/test)
endif() #WW_WITH_TESTS

add_custom_target(
    doc
    COMMAND doxygen
    COMMAND echo "Documentation built in ${CMAKE_SOURCE_DIR}/doc/"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc/
)
