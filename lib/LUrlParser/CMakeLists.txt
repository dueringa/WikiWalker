cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker-Libs)

add_library(lurlparser STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/LUrlParser.cpp
)

target_include_directories(lurlparser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
