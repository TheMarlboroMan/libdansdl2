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
include_directories("include")

set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

add_library(dansdl2_static STATIC ${SOURCE})
add_library(dansdl2_shared SHARED ${SOURCE})

set_target_properties(dansdl2_static PROPERTIES OUTPUT_NAME dansdl2)
set_target_properties(dansdl2_shared PROPERTIES OUTPUT_NAME dansdl2)

if(WIN32)
	add_compile_definitions(WINBUILD=1)

	target_compile_definitions(dansdl2_shared PUBLIC -DWINBUILD)
	target_compile_definitions(dansdl2_static PUBLIC -DWINBUILD)

	include_directories("${CMAKE_CURRENT_LIST_DIR}/../log/include/")
	
	add_library(lm SHARED IMPORTED)
	set_target_properties(lm PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/lm/lib/liblm.dll")
	
endif()


install(TARGETS dansdl2_shared DESTINATION lib)
install(TARGETS dansdl2_static DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
if(WIN32)
	target_link_libraries(dansdl2_static lm SDL2 SDL2_ttf SDL2_mixer SDL2_image freeglut glew32 glu32 opengl32 stdc++fs)
	target_link_libraries(dansdl2_shared lm SDL2 SDL2_ttf SDL2_mixer SDL2_image freeglut glew32 glu32 opengl32 stdc++fs)
endif()

