cmake_minimum_required(VERSION 3.5.1)

project(libdansdl2)

set(CMAKE_BUILD_TYPE Release)

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#TODO: Should also allow for c++14
add_compile_options(-std=c++11)

#TODO: Add the rest of flags...
#y_n_choice "With debug" "DEBUG=-g" "#DEBUG=-g"
#debug=$__retval;

#The cache thing means that this is user settable from the command line.
set(LOG_DIR "../log" CACHE STRING "Directory where the log repository is (-DLOG_DIR=xxx)")
include_directories(${LOG_DIR})

set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/src");

add_library(dansdl2 STATIC ${SOURCE})
