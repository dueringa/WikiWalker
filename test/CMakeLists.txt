cmake_minimum_required(VERSION 2.8.11)
project(wikiwalker_tests)

find_package(PkgConfig)
find_package(CURL REQUIRED)
pkg_check_modules(UT REQUIRED unittest++)

#warning, will be different on debian and gentoo
include_directories(
	${UT_INCLUDE_DIRS}
	${CURL_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/
)
add_definitions(-std=c++11)

#won't work with cmake-built
set(LINK_OPTIONS ../src/WikiWalker.o ../src/Article.o ../src/ToJsonWriter.o ../src/CurlWikiGrabber.o ../src/CurlUrlCreator.o ../src/JsonToArticleConverter.o ../src/ArticleCollection.o ../lib/jsoncpp/jsoncpp.o)

# Define the CXX sources
set ( CXX_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CollectionTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CurlUrlCreatorTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ArticleTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/JsonToArticleConverterTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToJsonWriterTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ToGraphvizWriterTests.cpp
)

add_executable(run_tests
	${TESTED_SRCS}
	${CXX_SRCS}
)

target_link_libraries(run_tests
	${UT_LDFLAGS}
	${CURL_LIBRARIES}
)
