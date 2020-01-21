
set(Ruqola_widgets_SRCS
    ruqolamainwindow.cpp
    ruqolacentralwidget.cpp
    ruqolaloginwidget.cpp
    ruqolamainwidget.cpp
    )

set(Ruqola_widgets_dialog_SRCS
    dialogs/serverinfowidget.cpp
    dialogs/serverinfodialog.cpp
    dialogs/uploadfiledialog.cpp
    dialogs/uploadfilewidget.cpp
    dialogs/channelinfowidget.cpp
    dialogs/channelinfodialog.cpp
    dialogs/searchchanneldialog.cpp
    dialogs/searchchannelwidget.cpp
    dialogs/modifystatusdialog.cpp
    dialogs/modifystatuswidget.cpp
    )

set(Ruqola_channellist_SRCS
    channellist/channellistwidget.cpp
    channellist/statuscombobox.cpp
    channellist/channellistview.cpp
    )

set(Ruqola_roomwidget_SRCS
    room/roomwidget.cpp
    room/roomheaderwidget.cpp
    room/messagelistview.cpp
    room/messagelineedit.cpp
    room/messagelistdelegate.cpp
    room/messagelinewidget.cpp
    )

ecm_qt_declare_logging_category(Ruqola_widgets_SRCS HEADER ruqolawidgets_debug.h IDENTIFIER RUQOLAWIDGETS_LOG CATEGORY_NAME org.kde.ruqola.widgets)

add_library(libruqolawidgets
    ${Ruqola_widgets_SRCS}
    ${Ruqola_widgets_dialog_SRCS}
    ${Ruqola_channellist_SRCS}
    ${Ruqola_roomwidget_SRCS}
    )

generate_export_header(libruqolawidgets BASE_NAME libruqolawidgets)


target_link_libraries(libruqolawidgets
    Qt5::Gui
    Qt5::Widgets
    KF5::I18n
    KF5::ConfigCore
    KF5::XmlGui
    KF5::KIOWidgets
    KF5::WidgetsAddons
    librocketchatrestapi-qt5
    libruqolacore
)

if (NOT WIN32)
    target_link_libraries(libruqolawidgets Qt5::DBus)
endif()


set_target_properties(libruqolawidgets
    PROPERTIES OUTPUT_NAME ruqolawidgets VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
    )

if (BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
    add_subdirectory(channellist/autotests)
    add_subdirectory(room/autotests)
    add_subdirectory(dialogs/autotests)
endif()

install(TARGETS libruqolawidgets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
