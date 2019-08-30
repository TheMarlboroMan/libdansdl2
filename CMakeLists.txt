cmake_minimum_required(VERSION 3.5.1)

project(libdansdl2)

set(CMAKE_BUILD_TYPE Release)

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#TODO: Should also allow for c++14
add_compile_options(-std=c++11)

#TODO: Add the rest of flags...

#TODO: This should be given in the command line
include_directories(../log/)

set(SOURCE "")

add_subdirectory("${PROJECT_SOURCE_DIR}/audio")
add_subdirectory("${PROJECT_SOURCE_DIR}/input")
add_subdirectory("${PROJECT_SOURCE_DIR}/tools")
add_subdirectory("${PROJECT_SOURCE_DIR}/video")

add_library(dansdl2 STATIC ${SOURCE})
