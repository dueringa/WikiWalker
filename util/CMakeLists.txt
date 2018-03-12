cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker)

add_executable(cachefilter
    FilterAnalyzedArticles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ToGraphvizWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ArticleCollection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/CacheJsonToArticleConverter.cpp
)

target_include_directories(cachefilter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(cachefilter jsoncpp lww)