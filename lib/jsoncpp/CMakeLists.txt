cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker-Libs)

add_library(jsoncpp STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp.cpp
)

target_include_directories(jsoncpp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
