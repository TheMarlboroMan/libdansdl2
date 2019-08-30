add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/box")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/line")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/point")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/polygon")

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/primitive_representation.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/primitive_representation.h
   PARENT_SCOPE
)