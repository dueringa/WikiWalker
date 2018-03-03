# version.cmake
#
# You need to pass SRC and BIN for this to work
# SRC is the source directory which contains version.h.in
# BIN is the binary directory where to write version.h to

# thanks to https://cmake.org/pipermail/cmake/2010-July/038015.html 

find_package(Git QUIET)

if(Git_FOUND)
    execute_process(
        COMMAND
        ${GIT_EXECUTABLE} -C ${SRC} describe --always --dirty --tags
        RESULT_VARIABLE GIT_RETCODE
        OUTPUT_VARIABLE GIT_OUTPUT_VERSION
        ERROR_VARIABLE GIT_RAN_ERROR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else() # Git_FOUND
    message("Git not found, won't be able to autogenerate version")
endif() # Git_FOUND

# exit status 0 means successful
if(NOT Git_FOUND OR GIT_RETCODE)
    set(GIT_RAN_SUCCESSFULLY 0)
else() # GIT_RETCODE
    set(GIT_RAN_SUCCESSFULLY 1)
endif() #GIT_RETCODE

# also, project_...?
# overwriting version.h in ok (?), I want the generated one to be preferred
# and I guess include_directories in only appended...
configure_file(
    "${SRC}/version.h.in"
    "${BIN}/version.h"
)
