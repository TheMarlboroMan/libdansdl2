add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/group")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/primitive")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/raster")

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/representation.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/representation.h
   PARENT_SCOPE
)