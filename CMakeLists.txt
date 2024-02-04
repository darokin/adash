cmake_minimum_required(VERSION 3.0.0)
project(adash VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
include(FindCurses)


find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(adash main.cpp screen.cpp widget.cpp widgetManager.cpp)

target_link_libraries(adash ${CURSES_LIBRARY})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
