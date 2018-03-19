cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker)

add_custom_target(
    doc
    COMMAND doxygen
    COMMAND echo "Documentation built in ${CMAKE_CURRENT_SOURCE_DIR}/"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
)
