project( forge )
set( SOURCES forge.cpp entry.cpp )

add_executable( forge ${SOURCES} )
target_link_libraries( forge springdvs boost_program_options pthread)

