cmake_minimum_required(VERSION 2.8.11)
project(wikiwalker)

find_package(PkgConfig)
find_package(CURL REQUIRED)
pkg_check_modules(UT REQUIRED unittest++)

include_directories(
	${UT_INCLUDE_DIRS}
	${CURL_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/
)

add_definitions(-std=c++11)

#ugly: have to specify sources again, and they will be built again
set( TESTED_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/../src/Article.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/ToJsonWriter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/ToGraphvizWriter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/CurlUrlCreator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/JsonToArticleConverter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../src/ArticleCollection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/jsoncpp.cpp
)

# Define the CXX sources
set ( CXX_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/ArticleTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CollectionTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CurlUrlCreatorTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/JsonToArticleConverterTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ToGraphvizWriterTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ToJsonWriterTests.cpp
)

add_executable(run_tests
	${TESTED_SRCS}
	${CXX_SRCS}
)

target_link_libraries(run_tests
	${UT_LDFLAGS}
	${CURL_LIBRARIES}
)
