cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin" )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/shared/include" )
add_subdirectory( shared )

add_subdirectory( nodes/root )
add_subdirectory( nodes/trusted )
add_subdirectory( nodes/org )

add_subdirectory(testing)
