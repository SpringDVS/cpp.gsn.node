project( catch_lnp )

include_directories( 
    "${CMAKE_CURRENT_SOURCE_DIR}/include" 
    "${CMAKE_CURRENT_SOURCE_DIR}/../nodes" 
)

set( NDIR "${CMAKE_CURRENT_SOURCE_DIR}/../nodes" )

set( SRC_SHARED 
    shared/mod_crypto.cpp 
    shared/mod_activity.cpp 
    shared/mod_activity_log.cpp
    shared/mod_netspace_url.cpp
    shared/mod_netnode.cpp 
    shared/mod_netspace_table.cpp
    shared/mod_metaspace_gsn.cpp
    shared/mod_dvsp_packet.cpp
)

set( SRC_NODES
    nodes/root/mod_root_protocol.cpp 
    nodes/root/mod_node_root.cpp 
)
set( SOURCES entry.cpp ${SRC_SHARED} ${SRC_NODES} )

add_executable(  catch_lnp ${SOURCES} )
target_link_libraries( catch_lnp springdvs )

