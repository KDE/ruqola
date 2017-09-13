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

####
set(simulatortest_SRCS simulator.cpp fakewebsocket.cpp)

add_executable(simulatortest ${simulatortest_SRCS})

target_link_libraries(simulatortest
    Qt5::Widgets
    KF5::KIOWidgets
    Qt5::WebSockets
    libruqolacore
    )
