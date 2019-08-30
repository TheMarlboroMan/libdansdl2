add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/bitmap")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ttf")

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/raster_representation.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/raster_representation.h
   PARENT_SCOPE
)