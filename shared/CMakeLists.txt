project( springdvs )
set( SOURCES 
        crypto.cpp 
        activity/activity.cpp
        activity/log_transaction.cpp
        activity/activity_log.cpp 
        activity/log_segment.cpp

        infrastructure/netnode.cpp
        infrastructure/netspace_table.cpp
        infrastructure/netspace_url.cpp
        infrastructure/metaspace_gsn.cpp

        protocol/dvsp_packet.cpp
)

add_library( springdvs SHARED ${SOURCES} )
target_link_libraries( springdvs ssl crypto sqlite3 boost_system boost_thread boost_regex)
