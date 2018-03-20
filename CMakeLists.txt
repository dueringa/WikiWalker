cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    # disable language extensions
    # this prevents MSVC from allowing binding temporaries to references
    # not sure why CMAKE_CXX_EXTENSIONS OFF doesn't handle this
    # related to #17068 in cmake?
    add_compile_options("/Za")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")

option(WW_WITH_CODECOV "also build code coverage" OFF)

if(WW_WITH_CODECOV)
    include(CodeCoverage)
    append_coverage_compiler_flags()

    setup_target_for_coverage(
        NAME testrunner_coverage
        EXECUTABLE execute_tests
        DEPENDENCIES execute_tests
    )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(WW_WITH_TESTS "also build tests" OFF)

enable_testing()

# Projects
add_subdirectory(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_SOURCE_DIR}/util)

if(WW_WITH_TESTS)
    add_subdirectory(${CMAKE_SOURCE_DIR}/test)
endif() #WW_WITH_TESTS

add_subdirectory(doc)

file(GLOB formatsources inc/*.h src/*.cpp test/*.cpp)
file(GLOB checksources src/*.cpp )

add_custom_target(
    clang-format-source
    COMMAND clang-format -i ${formatsources}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(
    clang-check-source
    COMMAND echo "clang-check..."
    COMMAND clang-check -analyze -p ${CMAKE_BINARY_DIR} ${checksources}
    COMMAND echo "clang-tidy..."
    COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} ${checksources}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
