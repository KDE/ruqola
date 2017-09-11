set(expandjsontest_SRCS expandjson.cpp)

add_executable(expandjsontest ${expandjsontest_SRCS})

target_link_libraries(expandjsontest
    Qt5::Widgets
    )

####

set(loadroomcachetest_SRCS loadroomcache.cpp)

add_executable(loadroomcachetest ${loadroomcachetest_SRCS})

target_link_libraries(loadroomcachetest
    Qt5::Widgets
    KF5::KIOWidgets
    )
