cmake_minimum_required(VERSION 2.8.11)
project(WikiWalker)

include_directories(
    ../lib/jsoncpp/
)

# Compiler options
add_definitions(-Wall)
add_definitions(-Wextra)
add_definitions(-Wshadow)
add_definitions(-std=c++11)
add_definitions(-Werror=return-type)

# Linker options

# Define the CXX sources
set ( CXX_SRCS
    ${CMAKE_SOURCE_DIR}/CurlWikiGrabber.cpp
    ${CMAKE_SOURCE_DIR}/CurlUrlCreator.cpp
    ${CMAKE_SOURCE_DIR}/JsonToArticleConverter.cpp
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/ArticleCollection.cpp
    ${CMAKE_SOURCE_DIR}/WikiWalker.cpp
    ${CMAKE_SOURCE_DIR}/ToJsonWriter.cpp
    ${CMAKE_SOURCE_DIR}/ToGraphvizWriter.cpp
    ${CMAKE_SOURCE_DIR}/Article.cpp
    ${CMAKE_SOURCE_DIR}/../lib/jsoncpp/jsoncpp.cpp
)

add_executable(walker ${CXX_SRCS})

target_link_libraries(walker curl)
