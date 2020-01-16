set(expandjsontest_SRCS expandjson.cpp)

add_executable(expandjsontest ${expandjsontest_SRCS})

target_link_libraries(expandjsontest
    Qt5::Widgets
    )

# ==================================================================== #
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    ItemViews
)
set(unicodeemoticongui_SRCS unicodeemoticongui.cpp)

qt5_add_resources(unicodeemoticongui_SRCS ../src/core/ruqolacore.qrc)

add_executable(unicodeemoticongui ${unicodeemoticongui_SRCS})

target_link_libraries(unicodeemoticongui
    Qt5::Widgets
    KF5::ItemViews
    libruqolacore
    )
