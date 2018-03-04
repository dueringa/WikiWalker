cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker VERSION 0.2.0)

set(walker_version "v${PROJECT_VERSION}")

add_custom_target(
    gitversion
    ${CMAKE_COMMAND}
    -D SRC=${CMAKE_CURRENT_SOURCE_DIR}
    -D BIN=${CMAKE_CURRENT_BINARY_DIR}
    -D walker_version=${walker_version}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/version.cmake
)

find_package(CURL REQUIRED)

find_package(Boost
    COMPONENTS program_options
)

set(ww_cmdparse_helpstring "Command line parser to use (boostpo or getopt)")

set(WW_COMMANDLINEPARSER "" CACHE STRING ${ww_cmdparse_helpstring})

# empty list, otherwise we end up appending forever
set_property(CACHE WW_COMMANDLINEPARSER PROPERTY STRINGS)

# if boost found
if(Boost_PROGRAM_OPTIONS_FOUND)
    set_property(CACHE WW_COMMANDLINEPARSER APPEND PROPERTY STRINGS "boostpo")
    # set default value to boostpo
    if(WW_COMMANDLINEPARSER STREQUAL "")
        set(WW_COMMANDLINEPARSER "boostpo" CACHE STRING
            ${ww_cmdparse_helpstring} FORCE)
    endif()
endif() #Boost_PROGRAM_OPTIONS_FOUND

message("Checking whether getopt is available.")
include(CheckFunctionExists)
include(CheckIncludeFile)
check_function_exists(getopt_long  HAS_GETOPT_FUNCTION)
check_include_file(getopt.h  HAS_GETOPT_HEADER)

if(HAS_GETOPT_FUNCTION AND HAS_GETOPT_HEADER)
    set_property(CACHE WW_COMMANDLINEPARSER APPEND PROPERTY STRINGS "getopt")
    # set default value to getopt
    if(WW_COMMANDLINEPARSER STREQUAL "")
        set(WW_COMMANDLINEPARSER "getopt" CACHE STRING
            ${ww_cmdparse_helpstring} FORCE)
    endif()
endif() #HAS_GETOPT_FUNCTION AND HAS_GETOPT_HEADER

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

# Define commonly used source files (with tests)
add_library(WikiWalkerSource OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/CurlWikiGrabber.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurlUrlCreator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WikimediaJsonToArticleConverter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CacheJsonToArticleConverter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ArticleCollection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WikiWalker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToJsonWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToGraphvizWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Article.cpp
)

add_executable(walker
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/jsoncpp.cpp
    $<TARGET_OBJECTS:WikiWalkerSource>
)

if(${WW_USE_BOOST_PO})
    target_sources(walker PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/BoostPoCommandLineParser.cpp
    )
elseif(${WW_USE_GETOPT})
    target_sources(walker PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/GetoptCommandLineParser.cpp
    )
endif()

# Compiler options
# TODO: Sadly, won't activate on llvm...
if(CMAKE_COMPILER_IS_GNUCXX)
# TODO: is PRIVATE correct?
target_compile_options(walker PRIVATE
    -Wall -Wextra -Wshadow -Werror=return-type)
endif() # CMAKE_COMPILER_IS_GNUCXX

set_property(TARGET walker WikiWalkerSource PROPERTY CXX_STANDARD 11)
set_property(TARGET walker WikiWalkerSource PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET walker WikiWalkerSource PROPERTY CXX_EXTENSIONS OFF)

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

add_dependencies(walker gitversion)
