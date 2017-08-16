ecm_create_qm_loader(ruqola_QM_LOADER ruqola_qt)

set (Ruqola_core_srcs
    messagemodel.cpp
    message.cpp
    roommodel.cpp
    roomfilterproxymodel.cpp
    room.cpp
    roomwrapper.cpp
    ddpclient.cpp
    ruqola.cpp
    ruqolautils.cpp
    rocketchatbackend.cpp
    notification.cpp
    messagequeue.cpp
    authentication.cpp
    rocketchatmessage.cpp
    typingnotification.cpp
    changetemporarystatus.cpp
    ${ruqola_QM_LOADER}
)

ecm_qt_declare_logging_category(Ruqola_core_srcs HEADER ruqola_debug.h IDENTIFIER RUQOLA_LOG CATEGORY_NAME org.kde.ruqola)

add_library(libruqolacore ${Ruqola_core_srcs})

generate_export_header(libruqolacore BASE_NAME libruqolacore)

target_link_libraries(libruqolacore
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::WebSockets
    Qt5::Network
    Qt5::NetworkAuth
    KF5::CoreAddons
)

set_target_properties(libruqolacore
    PROPERTIES OUTPUT_NAME ruqolacore VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
    )


qt5_add_resources(RuqolaResources qml/qml.qrc)

add_executable(ruqola main.cpp ${RuqolaResources})

target_link_libraries(ruqola libruqolacore)

install(TARGETS ruqola ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(TARGETS libruqolacore ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
