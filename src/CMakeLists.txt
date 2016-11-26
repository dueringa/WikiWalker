cmake_minimum_required(VERSION 2.8.11)
project(WikiWalker)

SET(walker_version "git-prerelease")

execute_process(
	COMMAND	git -C ${CMAKE_CURRENT_SOURCE_DIR} describe --always --dirty --tags
	RESULT_VARIABLE GIT_RETCODE
	OUTPUT_VARIABLE GIT_OUTPUT_VERSION
	ERROR_VARIABLE GIT_RAN_ERROR
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

#message("Git result: ${GIT_RETCODE} / ${GIT_OUTPUT_VERSION} / ${GIT_RAN_ERROR}")

# exit status 0 means successful
if (GIT_RETCODE)
	set(GIT_RAN_SUCCESSFULLY 0)
else (GIT_RETCODE)
	set(GIT_RAN_SUCCESSFULLY 1)
endif (GIT_RETCODE)

# also, project_...?
# overwriting version.h in ok (?), I want the generated one to be preferred
# and I guess include_directories in only appended...
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

find_package(CURL REQUIRED)
find_package(Boost
	REQUIRED
	COMPONENTS program_options
)

## huh, path spec is relative to... ???
include_directories(
	../lib/jsoncpp/
	${CURL_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
	${Boost_INCLUDE_DIR}
)

# Compiler options
# TODO: What's the difference between this and SET_SOURCE_FILES_PROPERTIES?
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
    ${CMAKE_CURRENT_SOURCE_DIR}/WikimediaJsonToArticleConverter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CacheJsonToArticleConverter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ArticleCollection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WikiWalker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToJsonWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToGraphvizWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Article.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BoostPoCommandLineParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/jsoncpp.cpp
)

add_executable(walker ${CXX_SRCS})

target_link_libraries(walker
	${CURL_LIBRARIES}
	${Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE}
)

install (TARGETS walker DESTINATION bin)

enable_testing()
add_test(NAME versiondisplay
	COMMAND walker -v)
