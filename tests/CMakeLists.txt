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
qt5_add_resources(simulatortestresources ../src/qml/qml.qrc)

add_executable(simulatortest ${simulatortest_SRCS} ${simulatortestresources})

target_link_libraries(simulatortest
    Qt5::Widgets
    KF5::KIOWidgets
    Qt5::WebSockets
    libruqolacore
    )

####
set(messagemodelguitest_SRCS messagemodelgui.cpp)
qt5_add_resources(messagemodelguitestresources ../src/qml/qml.qrc messagemodelgui.qrc)

add_executable(messagemodelguitest ${messagemodelguitest_SRCS} ${messagemodelguitestresources})

target_link_libraries(messagemodelguitest
    Qt5::Widgets
    KF5::KIOWidgets
    Qt5::WebSockets
    libruqolacore
    )

####
set(roommodelguitest_SRCS roommodelgui.cpp)
qt5_add_resources(roommodelguitestresources ../src/qml/qml.qrc ) # roommodelgui.qrc)

add_executable(roommodelguitest ${roommodelguitest_SRCS} ${roommodelguitestresources})

target_link_libraries(roommodelguitest
    Qt5::Widgets
    KF5::KIOWidgets
    Qt5::WebSockets
    libruqolacore
    )

####
set(aboutdatatest_SRCS aboutdatatest.cpp)
qt5_add_resources(aboutdatatestresources  aboutdatatest.qrc ../src/qml/qml.qrc )

add_executable(aboutdatatest ${aboutdatatest_SRCS} ${aboutdatatestresources})

target_link_libraries(aboutdatatest
    Qt5::Widgets
    KF5::KIOWidgets
    Qt5::WebSockets
    libruqolacore
    )

####
set (qmlfiletest_SRCS qmltest.cpp)
qt5_add_resources(qmlfiletestResources ../src/qml/qml.qrc)

add_executable(qmlfiletest ${qmlfiletest_SRCS} ${qmlfiletestResources})

target_link_libraries(qmlfiletest
    libruqolacore
    )
