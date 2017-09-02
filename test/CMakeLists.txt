cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker)

#for pkg-config based stuff
#find_package(PkgConfig)
#pkg_check_modules(UT UnitTest++)
#set(UT_INCLUDE_DIRS "" CACHE PATH "unittest++ include path")
#set(UT_LDFLAGS "" CACHE STRING "unittest++ library flag")

find_package(CURL REQUIRED)
find_package(UnitTest++ REQUIRED)

include_directories(
    ${UT_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/
    ${UTPP_INCLUDE_DIRS}
)

add_executable(run_tests
    $<TARGET_OBJECTS:WikiWalkerSource>
    ${CMAKE_CURRENT_SOURCE_DIR}/ArticleTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CollectionTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CacheJsonToArticleConverterTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurlUrlCreatorTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WikimediaJsonToArticleConverterTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToGraphvizWriterTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToJsonWriterTests.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/jsoncpp.cpp
)

set_property(TARGET run_tests PROPERTY CXX_STANDARD 11)
set_property(TARGET run_tests PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET run_tests PROPERTY CXX_EXTENSIONS OFF)

target_link_libraries(run_tests
    ${UT_LDFLAGS}
    ${CURL_LIBRARIES}
    "UnitTest++"
)
