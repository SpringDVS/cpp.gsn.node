project( lnroot )
set( SOURCES entry.cpp )

add_executable( lnroot ${SOURCES} )
target_link_libraries( lnroot lowland )
