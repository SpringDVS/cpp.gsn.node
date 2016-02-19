project( lntrusted )
set( SOURCES entry.cpp )

add_executable( lntrusted ${SOURCES} )
target_link_libraries( lntrusted springdvs )
