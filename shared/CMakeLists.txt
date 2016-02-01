project( lowland )
set( SOURCES crypto.cpp activity.cpp activity_log.cpp )

add_library( lowland SHARED ${SOURCES} )
target_link_libraries( lowland ssl crypto )
