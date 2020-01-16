
set(Ruqola_widgets_SRCS
    ruqolamainwindow.cpp
    ruqolacentralwidget.cpp
    )

set(Ruqola_widgets_dialog_SRCS
    dialogs/serverinfowidget.cpp
    )

ecm_qt_declare_logging_category(Ruqola_widgets_SRCS HEADER ruqolawidgets_debug.h IDENTIFIER RUQOLAWIDGETS_LOG CATEGORY_NAME org.kde.ruqola.widgets)

add_library(libruqolawidgets
    ${Ruqola_widgets_SRCS}
    ${Ruqola_widgets_dialog_SRCS}
    )

generate_export_header(libruqolawidgets BASE_NAME libruqolawidgets)


target_link_libraries(libruqolawidgets
    Qt5::Gui
    Qt5::Widgets
    KF5::I18n
    KF5::ConfigCore
    KF5::XmlGui
    librocketchatrestapi-qt5
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
endif()

install(TARGETS libruqolawidgets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
