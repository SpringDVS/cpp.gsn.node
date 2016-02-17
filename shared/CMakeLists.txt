project( lowland )
set( SOURCES 
        crypto.cpp 
        activity/activity.cpp
        activity/log_transaction.cpp
        activity/activity_log.cpp 
        activity/log_segment.cpp)

add_library( lowland SHARED ${SOURCES} )
target_link_libraries( lowland ssl crypto )
