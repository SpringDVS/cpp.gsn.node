project( catch_lnp )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )

set( SRC_SHARED shared/mod_activity.cpp shared/mod_crypto.cpp )
set( SOURCES entry.cpp ${SRC_SHARED} )

add_executable(  catch_lnp ${SOURCES} )
target_link_libraries( catch_lnp lowland)

