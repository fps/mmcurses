cmake_minimum_required (VERSION 2.8)

project(mmcurses)
set(PROJECT_VERSION "0.1.0")

set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
add_custom_target(dist
    COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
        | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_definitions(-DPROJECT_VERSION=\"${PROJECT_VERSION}\")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wconversion -Werror -pthread -std=c++11 -fPIC -fmax-errors=1")

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    add_definitions("-DNDEBUG")
endif()

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(mmcurses src/application.cc)
target_link_libraries(mmcurses ncurses)

add_executable(mmcurses_application_example1 examples/mmcurses_application_example1.cc)
target_link_libraries(mmcurses_application_example1 mmcurses)

add_executable(mmcurses_application_example2 examples/mmcurses_application_example2.cc)
target_link_libraries(mmcurses_application_example2 mmcurses)

