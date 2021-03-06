cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker VERSION 0.2.0)

set(walker_version "v${PROJECT_VERSION}")

add_custom_target(
    gitversion
    ${CMAKE_COMMAND}
    -D SRC=${CMAKE_SOURCE_DIR}/inc/
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

configure_file(
    "${CMAKE_SOURCE_DIR}/inc/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

# Define commonly used source files (with tests)
add_library(WikiWalkerSource OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/Article.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ArticleCollection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParserBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParserUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurlUrlCreator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurlWikiGrabber.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataOutputBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/JsonSerializer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StringUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToGraphvizWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Walker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WalkerException.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WikimediaApi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WikimediaJsonToArticleConverter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WikiWalker.cpp

    ${CMAKE_SOURCE_DIR}/inc/Article.h
    ${CMAKE_SOURCE_DIR}/inc/ArticleCollection.h
    ${CMAKE_SOURCE_DIR}/inc/CommandLineParserBase.h
    ${CMAKE_SOURCE_DIR}/inc/CommandLineParserUtils.h
    ${CMAKE_SOURCE_DIR}/inc/CurlUrlCreator.h
    ${CMAKE_SOURCE_DIR}/inc/CurlWikiGrabber.h
    ${CMAKE_SOURCE_DIR}/inc/DataOutputBase.h
    ${CMAKE_SOURCE_DIR}/inc/JsonSerializer.h
    ${CMAKE_SOURCE_DIR}/inc/StringUtils.h
    ${CMAKE_SOURCE_DIR}/inc/ToGraphvizWriter.h
    ${CMAKE_SOURCE_DIR}/inc/Walker.h
    ${CMAKE_SOURCE_DIR}/inc/WalkerException.h
    ${CMAKE_SOURCE_DIR}/inc/WikimediaApi.h
    ${CMAKE_SOURCE_DIR}/inc/WikimediaJsonToArticleConverter.h
    ${CMAKE_SOURCE_DIR}/inc/WikiWalker.h
)

target_include_directories(WikiWalkerSource BEFORE PRIVATE
    ${CURL_INCLUDE_DIRS}
    $<TARGET_PROPERTY:jsoncpp,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:lurlparser,INTERFACE_INCLUDE_DIRECTORIES>
    ${CMAKE_SOURCE_DIR}/inc/
)

add_executable(walker
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    $<TARGET_OBJECTS:WikiWalkerSource>
)

if(${WW_USE_BOOST_PO})
    target_sources(walker PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/BoostPoCommandLineParser.cpp
        ${CMAKE_SOURCE_DIR}/inc/BoostPoCommandLineParser.h
    )
elseif(${WW_USE_GETOPT})
    target_sources(walker PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/GetoptCommandLineParser.cpp
        ${CMAKE_SOURCE_DIR}/inc/GetoptCommandLineParser.h
    )
endif()

# Compiler options
option(WW_EXTENDED_WARNINGS "Enable extended compiler warnings for gcc / clang")

if(WW_EXTENDED_WARNINGS)
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
        target_compile_options(WikiWalkerSource PRIVATE
            -Wall -Wextra -Wshadow -Werror=return-type)
        target_compile_options(walker PRIVATE
            -Wall -Wextra -Wshadow -Werror=return-type)
    endif() # clang or gcc

    if(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
        target_compile_options(WikiWalkerSource PRIVATE
            -Weverything -Wno-c++98-compat -Wno-padded)
        target_compile_options(walker PRIVATE
            -Weverything -Wno-c++98-compat -Wno-padded)
    endif() # clang
    
    if(MSVC)
        # Wall emits too many "(not) inlined" warnings
        target_compile_options(WikiWalkerSource PRIVATE /W4)
        target_compile_options(walker PRIVATE /W4)
    endif() #MSVC
endif() # WW_EXTENDED_WARNINGS

target_link_libraries(walker
    ${CURL_LIBRARIES}
    jsoncpp
    lurlparser
)

# if boost wanted
if(${WW_USE_BOOST_PO})
    target_include_directories(walker PRIVATE
        ${Boost_INCLUDE_DIR}
    )
    target_link_libraries(walker
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
    )
endif()

target_include_directories(walker PRIVATE
    ${CMAKE_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_BINARY_DIR}
)

install(TARGETS walker DESTINATION bin)

enable_testing()
add_test(NAME versiondisplay
    COMMAND walker -v)

add_dependencies(walker gitversion)
