cmake_minimum_required(VERSION 3.5.1)
project(libdansdl2)

option(BUILD_DEBUG "Build a debug release" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(${BUILD_DEBUG})
	message("DEBUG BUILD")
        set(CMAKE_BUILD_TYPE Debug)

else()
        set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#Add the include directories
#The cache thing means that this is user settable from the command line.
set(LOG_DIR "${PROJECT_SOURCE_DIR}/../log/include/" CACHE STRING "Directory where the log repository is (-DLOG_DIR=xxx)")
include_directories("include" ${LOG_DIR})

set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

add_library(dansdl2_static STATIC ${SOURCE})
set_target_properties(dansdl2_static PROPERTIES OUTPUT_NAME dansdl2)

add_library(dansdl2_shared SHARED ${SOURCE})
set_target_properties(dansdl2_shared PROPERTIES OUTPUT_NAME dansdl2)
	
install(TARGETS dansdl2_shared DESTINATION lib)
install(TARGETS dansdl2_static DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
