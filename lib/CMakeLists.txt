cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker-Libs)

add_library(jsoncpp STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp/jsoncpp.cpp
)

add_library(lurlparser STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/LUrlParser/LUrlParser.cpp
)

set(LIBVERSION_MAJOR 0)
set(LIBVERSION_MINOR 1)
set(LIBVERSION_PATCH 0)
set(libversion ${LIBVERSION_MAJOR}.${LIBVERSION_MINOR}.${LIBVERSION_PATCH})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/wikiwalker/inc/libwwconfigure.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/libwwconfigure.h
 )

# for the sake of IDE users, add the headers as well
add_library(lww 
    ${CMAKE_CURRENT_SOURCE_DIR}/wikiwalker/Article.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wikiwalker/inc/Article.h
    ${CMAKE_CURRENT_SOURCE_DIR}/wikiwalker/inc/WalkerException.h
    ${CMAKE_CURRENT_SOURCE_DIR}/wikiwalker/inc/libwwconfigure.h.in
)

set_target_properties(lww PROPERTIES SOVERSION ${libversion})

get_target_property(libwwtype lww TYPE)
include(GenerateExportHeader)
generate_export_header(lww)
if(libwwtype STREQUAL "STATIC_LIBRARY")
    target_compile_definitions(lww PUBLIC LWW_STATIC_DEFINE)
endif()

target_include_directories(lww PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/wikiwalker/inc/
    ${CMAKE_CURRENT_BINARY_DIR}
)

install(TARGETS lww DESTINATION lib)

target_include_directories(jsoncpp BEFORE PUBLIC jsoncpp)
target_include_directories(lurlparser PUBLIC LUrlParser)
