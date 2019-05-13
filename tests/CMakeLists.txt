set(expandjsontest_SRCS expandjson.cpp)

add_executable(expandjsontest ${expandjsontest_SRCS})

target_link_libraries(expandjsontest
    Qt5::Widgets
    )

# ==================================================================== #

#TODO reactivate it
#set(loadroomcachetest_SRCS loadroomcache.cpp)

#add_executable(loadroomcachetest ${loadroomcachetest_SRCS})

#target_link_libraries(loadroomcachetest
#    Qt5::Widgets
#    #TODO remove kurlrequester
#    KF5::KIOWidgets
#    )

# ==================================================================== #
set(messagemodelguitest_SRCS messagemodelgui.cpp)
qt5_add_resources(messagemodelguitestresources ../src/apps/qml/qml.qrc messagemodelgui.qrc)

add_executable(messagemodelguitest ${messagemodelguitest_SRCS} ${messagemodelguitestresources})

target_link_libraries(messagemodelguitest
    Qt5::Widgets
    Qt5::WebSockets
    libruqolacore
    )

# ==================================================================== #
set(roommodelguitest_SRCS roommodelgui.cpp)
qt5_add_resources(roommodelguitestresources ../src/apps/qml/qml.qrc messagemodelgui.qrc)

add_executable(roommodelguitest ${roommodelguitest_SRCS} ${roommodelguitestresources})

target_link_libraries(roommodelguitest
    Qt5::Widgets
    Qt5::WebSockets
    libruqolacore
    )

# ==================================================================== #
set(aboutdatatest_SRCS aboutdatatest.cpp)
qt5_add_resources(aboutdatatestresources  aboutdatatest.qrc ../src/apps/qml/qml.qrc )

add_executable(aboutdatatest ${aboutdatatest_SRCS} ${aboutdatatestresources})

target_link_libraries(aboutdatatest
    Qt5::Widgets
    Qt5::WebSockets
    libruqolacore
    )

# ==================================================================== #
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    ItemViews
)
set(unicodeemoticongui_SRCS unicodeemoticongui.cpp)

qt5_add_resources(unicodeemoticongui_SRCS ../src/ruqolacore/ruqolacore.qrc)

add_executable(unicodeemoticongui ${unicodeemoticongui_SRCS})

target_link_libraries(unicodeemoticongui
    Qt5::Widgets
    KF5::ItemViews
    libruqolacore
    )
