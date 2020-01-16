set(ruqola-widget_SRCS
	main.cpp
	)
qt5_add_resources(ruqola-widget_SRCS ruqolawidget.qrc)

add_executable(ruqola-widget ${ruqola-widget_SRCS})

target_link_libraries(ruqola-widget
    libruqolacore
    libruqolawidgets
    librocketchatrestapi-qt5
    KF5::Crash
    KF5::DBusAddons
    KF5::XmlGui
)

install(TARGETS ruqola-widget ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
