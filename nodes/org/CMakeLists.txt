project( lnorg )
set( SOURCES entry.cpp )

add_executable( lnorg ${SOURCES} )
target_link_libraries( lnorg springdvs )
