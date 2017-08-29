ecm_create_qm_loader(ruqola_QM_LOADER ruqola_qt)

set (Ruqola_restapi_SRCS
    restapi/restapirequest.cpp
    restapi/restapiutil.cpp
    )

set(Ruqola_ddpapi_SRCS
    ddpapi/ddpclient.cpp
    )

set (Ruqola_core_srcs
    messagemodel.cpp
    message.cpp
    roommodel.cpp
    roomfilterproxymodel.cpp
    room.cpp
    roomwrapper.cpp
    ruqola.cpp
    ruqolautils.cpp
    rocketchatbackend.cpp
    notification.cpp
    messagequeue.cpp
    authentication.cpp
    rocketchatmessage.cpp
    typingnotification.cpp
    changetemporarystatus.cpp
    usermodel.cpp
    user.cpp
    utils.cpp
    abstractwebsocket.cpp
    ruqolawebsocket.cpp
    messageattachment.cpp
    rocketchataccount.cpp
    rocketchataccountsettings.cpp
    usersforroommodel.cpp
    fakewebsocket.cpp
    avatarfilecache.cpp
    ${Ruqola_ddpapi_SRCS}
    ${Ruqola_restapi_SRCS}
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
    KF5::I18n
)

set_target_properties(libruqolacore
    PROPERTIES OUTPUT_NAME ruqolacore VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
    )


qt5_add_resources(RuqolaResources qml/qml.qrc ruqola.qrc)

add_executable(ruqola main.cpp ${RuqolaResources})

target_link_libraries(ruqola libruqolacore)

install(TARGETS ruqola ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(TARGETS libruqolacore ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
