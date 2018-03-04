cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker)

find_package(CURL REQUIRED)
find_package(UnitTest++ REQUIRED)

include_directories(
    ${UT_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/
    ${UTPP_INCLUDE_DIRS}
)

add_executable(execute_tests
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

set_property(TARGET execute_tests PROPERTY CXX_STANDARD 11)
set_property(TARGET execute_tests PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET execute_tests PROPERTY CXX_EXTENSIONS OFF)

target_link_libraries(execute_tests
    ${UT_LDFLAGS}
    ${CURL_LIBRARIES}
    "UnitTest++"
)

add_test(NAME unittest COMMAND execute_tests)
