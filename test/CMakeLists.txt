cmake_minimum_required(VERSION 3.1.0)
project(wikiwalker)

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
    ${UTPP_INCLUDE_DIRS}
)

#ugly: have to specify sources again, and they will be built again
#object libraries? https://cmake.org/cmake/help/v3.1/manual/cmake-buildsystem.7.html#object-libraries
set(TESTED_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Article.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ToJsonWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ToGraphvizWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/CurlUrlCreator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/WikimediaJsonToArticleConverter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/CacheJsonToArticleConverter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ArticleCollection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/jsoncpp.cpp
)

# Define the CXX sources
set(CXX_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/ArticleTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CollectionTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CacheJsonToArticleConverterTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurlUrlCreatorTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WikimediaJsonToArticleConverterTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToGraphvizWriterTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToJsonWriterTests.cpp
)

add_executable(run_tests
    ${TESTED_SRCS}
    ${CXX_SRCS}
)

set_property(TARGET run_tests PROPERTY CXX_STANDARD 11)
set_property(TARGET run_tests PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET run_tests PROPERTY CXX_EXTENSIONS OFF)

target_link_libraries(run_tests
    ${UT_LDFLAGS}
    ${CURL_LIBRARIES}
    "UnitTest++"
)
