cmake_minimum_required(VERSION 3.5.1)
project(libdansdl2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#library version
set(MAJOR_VERSION 3)
set(MINOR_VERSION 1)
set(PATCH_VERSION 2)

option(BUILD_STATIC "generate static release" OFF)
option(BUILD_SHARED "generate shared release" OFF)
option(BUILD_DEBUG "generate shared debug release" OFF)

if(${CMAKE_VERSION} VERSION_LESS "3.22.0")

	add_definitions(-DMAJOR_VERSION=${MAJOR_VERSION})
	add_definitions(-DMINOR_VERSION=${MINOR_VERSION})
	add_definitions(-DPATCH_VERSION=${PATCH_VERSION})
else()

	add_compile_definitions(MAJOR_VERSION=${MAJOR_VERSION})
	add_compile_definitions(MINOR_VERSION=${MINOR_VERSION})
	add_compile_definitions(PATCH_VERSION=${PATCH_VERSION})
endif()

message("will build ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}-${BUILD_VERSION}")

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#Add the include directories
#The cache thing means that this is user settable from the command line.
include_directories("include")

set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

if(${BUILD_STATIC})

	add_library(dansdl2_static STATIC ${SOURCE})
	target_compile_definitions(dansdl2_static PUBLIC "-DBUILD_VERSION=\"archive\"")
	set_target_properties(dansdl2_static PROPERTIES OUTPUT_NAME dansdl2)
	set_target_properties(dansdl2_static PROPERTIES COMPILE_FLAGS "-O3 -DNDEBUG")
	install(TARGETS dansdl2_static DESTINATION lib)
endif()

if(${BUILD_SHARED})

	add_library(dansdl2_shared SHARED ${SOURCE})
	target_compile_definitions(dansdl2_shared PUBLIC "-DBUILD_VERSION=\"lib\"")
	set_target_properties(dansdl2_shared PROPERTIES OUTPUT_NAME dansdl2)
	set_target_properties(dansdl2_shared PROPERTIES COMPILE_FLAGS "-O3 -DNDEBUG")
	install(TARGETS dansdl2_shared DESTINATION lib)
endif()

if(${BUILD_DEBUG})

	add_library(dansdl2_debug SHARED ${SOURCE})
	target_compile_definitions(dansdl2_debug PUBLIC "-DBUILD_VERSION=\"debug\"")
	target_compile_definitions(dansdl2_debug PUBLIC "-DLIBDANSDL2_DEBUG")
	set_target_properties(dansdl2_debug PROPERTIES OUTPUT_NAME dansdl2_debug)
	set_target_properties(dansdl2_debug PROPERTIES COMPILE_FLAGS "-O0 -g")
	install(TARGETS dansdl2_debug DESTINATION lib)
endif()

#windows specifics
if(WIN32)
	add_compile_definitions(WINBUILD=1)

	if(${BUILD_STATIC})
		target_compile_definitions(dansdl2_static PUBLIC -DWINBUILD)
		target_link_libraries(dansdl2_static lm SDL2 SDL2_ttf SDL2_mixer SDL2_image glu32 opengl32 stdc++fs)
	endif()

	if(${BUILD_SHARED})
		target_compile_definitions(dansdl2_shared PUBLIC -DWINBUILD)
		target_link_libraries(dansdl2_shared lm SDL2 SDL2_ttf SDL2_mixer SDL2_image glu32 opengl32 stdc++fs)
	endif()

	if(${BUILD_DEBUG})
		target_compile_definitions(dansdl2_debug PUBLIC -DWINBUILD)
		target_link_libraries(dansdl2_debug lm SDL2 SDL2_ttf SDL2_mixer SDL2_image glu32 opengl32 stdc++fs)
	endif()

	include_directories("${CMAKE_CURRENT_LIST_DIR}/../log/include/")

	add_library(lm SHARED IMPORTED)
	set_target_properties(lm PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/lm/lib/liblm.dll")

endif()

install(DIRECTORY include/ DESTINATION include)
if(WIN32)
#	target_link_libraries(dansdl2_static lm SDL2 SDL2_ttf SDL2_mixer SDL2_image freeglut glew32 glu32 opengl32 stdc++fs)
#	target_link_libraries(dansdl2_shared lm SDL2 SDL2_ttf SDL2_mixer SDL2_image freeglut glew32 glu32 opengl32 stdc++fs)

endif()


