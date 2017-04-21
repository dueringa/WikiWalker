cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker)

set(walker_version "v0.1.2")

execute_process(
    COMMAND
    git -C ${CMAKE_CURRENT_SOURCE_DIR} describe --always --dirty --tags
    RESULT_VARIABLE GIT_RETCODE
    OUTPUT_VARIABLE GIT_OUTPUT_VERSION
    ERROR_VARIABLE GIT_RAN_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

#message("Git result: ${GIT_RETCODE} / ${GIT_OUTPUT_VERSION} / ${GIT_RAN_ERROR}")

# exit status 0 means successful
if(GIT_RETCODE)
    set(GIT_RAN_SUCCESSFULLY 0)
else(GIT_RETCODE)
    set(GIT_RAN_SUCCESSFULLY 1)
endif(GIT_RETCODE)

# also, project_...?
# overwriting version.h in ok (?), I want the generated one to be preferred
# and I guess include_directories in only appended...
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

find_package(CURL REQUIRED)

find_package(Boost
    COMPONENTS program_options
)

set(WW_COMMANDLINEPARSER "" CACHE STRING "Command line parser to use (boostpo or getopt)")

# empty list, otherwise we end up appending forever
set_property(CACHE WW_COMMANDLINEPARSER PROPERTY STRINGS)

# if boost found
if(Boost_PROGRAM_OPTIONS_FOUND)
    set_property(CACHE WW_COMMANDLINEPARSER APPEND PROPERTY STRINGS "boostpo")
endif(Boost_PROGRAM_OPTIONS_FOUND)

message("Checking whether getopt is available.")
include(CheckFunctionExists)
include(CheckIncludeFile)
check_function_exists(getopt_long  HAS_GETOPT_FUNCTION)
check_include_file(getopt.h  HAS_GETOPT_HEADER)

if(HAS_GETOPT_FUNCTION AND HAS_GETOPT_HEADER)
    set_property(CACHE WW_COMMANDLINEPARSER APPEND PROPERTY STRINGS "getopt")
endif(HAS_GETOPT_FUNCTION AND HAS_GETOPT_HEADER)

if("${WW_COMMANDLINEPARSER}" STREQUAL "boostpo")
    set(WW_USE_BOOST_PO TRUE)
elseif("${WW_COMMANDLINEPARSER}" STREQUAL "getopt")
    set(WW_USE_GETOPT TRUE)
else()
    message(SEND_ERROR "Must use either getopt or boost. Make sure \
either is available and set WW_COMMANDLINEPARSER")
endif()

## huh, path spec is relative to... ???
include_directories(
    ../lib/jsoncpp/
    ${CURL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

# Define the CXX sources
set(CXX_SRCS
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
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/jsoncpp.cpp
)

if(${WW_USE_BOOST_PO})
    list(APPEND CXX_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/BoostPoCommandLineParser.cpp
    )
elseif(${WW_USE_GETOPT})
    list(APPEND CXX_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/GetoptCommandLineParser.cpp
    )
endif()

add_executable(walker ${CXX_SRCS})

# Compiler options
# TODO: Sadly, won't activate on llvm...
if(CMAKE_COMPILER_IS_GNUCXX)
# TODO: is PRIVATE correct?
target_compile_options(walker PRIVATE -Wall
                       -Wextra -Wshadow -Werror=return-type)
endif(CMAKE_COMPILER_IS_GNUCXX)

set_property(TARGET walker PROPERTY CXX_STANDARD 11)
set_property(TARGET walker PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET walker PROPERTY CXX_EXTENSIONS OFF)

target_link_libraries(walker
    ${CURL_LIBRARIES}
)

# if boost wanted
if(${WW_USE_BOOST_PO})
    include_directories(
        ${Boost_INCLUDE_DIR}
    )
    target_link_libraries(walker
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
    )
endif()

install(TARGETS walker DESTINATION bin)

enable_testing()
add_test(NAME versiondisplay
    COMMAND walker -v)
