cmake_minimum_required(VERSION 3.1.0)
project(WikiWalker)

find_package(Doxygen)

if(DOXYGEN_FOUND)
    add_custom_target(
        doc
        COMMAND ${DOXYGEN_EXECUTABLE}
        COMMAND echo "Documentation built in ${CMAKE_CURRENT_SOURCE_DIR}/"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    )
else()
    message(WARNING "Doxygen was not found. doc target won't be generated")
endif()
