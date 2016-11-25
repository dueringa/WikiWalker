cmake_minimum_required(VERSION 2.8.11)
project(WikiWalker)

SET(walker_version "git-prerelease")

# also, project_...?
# overwriting version.h in ok (?), I want the generated one to be preferred
# and I guess include_directories in only appended...
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/version.h"
)

find_package(CURL REQUIRED)
include_directories(
	../lib/jsoncpp/
	${CURL_INCLUDE_DIRS}
)

add_custom_command(
	OUTPUT version.txt
	COMMAND git -C ${CMAKE_CURRENT_SOURCE_DIR} describe --always --dirty --tags > version.txt
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
    ${CMAKE_CURRENT_SOURCE_DIR}/CurlWikiGrabber.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurlUrlCreator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/JsonToArticleConverter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ArticleCollection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WikiWalker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToJsonWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToGraphvizWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Article.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/jsoncpp.cpp
)

add_executable(walker ${CXX_SRCS} version.txt)

target_link_libraries(walker ${CURL_LIBRARIES})

install (TARGETS walker DESTINATION bin)
