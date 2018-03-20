
set (Ruqola_restapi_SRCS
    restapi/restapirequest.cpp
    restapi/restapiutil.cpp
    restapi/restapimethod.cpp
    restapi/uploadfilejob.cpp
    restapi/serverinfojob.cpp
    restapi/restapiabstractjob.cpp
    restapi/owninfojob.cpp
    restapi/getavatarjob.cpp
    restapi/logoutjob.cpp
    restapi/loginjob.cpp
    restapi/privateinfojob.cpp
    restapi/channellistjob.cpp
    restapi/starmessagejob.cpp
    restapi/downloadfilejob.cpp
    restapi/pinmessagejob.cpp
    restapi/postmessagejob.cpp
    restapi/channels/changechanneltopicjob.cpp
    restapi/channels/changechannelannouncementjob.cpp
    restapi/groups/changegroupstopicjob.cpp
    restapi/groups/changegroupsannouncementjob.cpp
    )

set(Ruqola_ddpapi_SRCS
    ddpapi/ddpclient.cpp
    )

set (Ruqola_model_core_srcs    
    model/messagemodel.cpp
    model/roommodel.cpp
    model/roomfilterproxymodel.cpp

    model/usersforroommodel.cpp
    model/usersforroomfilterproxymodel.cpp

    model/usersmodel.cpp

    model/usercompleterfilterproxymodel.cpp
    model/usercompletermodel.cpp

    model/statusmodel.cpp

    model/filesforroommodel.cpp
    model/filesforroomfilterproxymodel.cpp

    model/searchchannelmodel.cpp
    model/searchchannelfilterproxymodel.cpp

    model/inputcompletermodel.cpp

    model/loginmethodmodel.cpp

    model/searchmessagemodel.cpp
    model/searchmessagefilterproxymodel.cpp

    model/rocketchataccountmodel.cpp
    model/rocketchataccountfilterproxymodel.cpp

    )

set(Ruqola_plugins_srcs
    plugins/pluginauthenticationinterface.cpp
    plugins/pluginauthentication.cpp
    )

set(Ruqola_aboutdata_srcs
    aboutdata/qmlaboutdata.cpp
    aboutdata/qmlaboutdataauthormodel.cpp
    )

set (Ruqola_core_srcs
    ${Ruqola_model_core_srcs}
    ${Ruqola_plugins_srcs}
    ${Ruqola_aboutdata_srcs}
    ${Ruqola_ddpapi_SRCS}
    ${Ruqola_restapi_SRCS}
    authenticationinfo.cpp
    message.cpp
    room.cpp
    roomwrapper.cpp
    ruqola.cpp
    ruqolautils.cpp
    rocketchatbackend.cpp
    notification.cpp
    messagequeue.cpp
    rocketchatmessage.cpp
    typingnotification.cpp
    changetemporarystatus.cpp
    user.cpp
    utils.cpp
    emoji.cpp
    emojimanager.cpp
    clipboardproxy.cpp
    otr.cpp
    otrmanager.cpp
    abstractwebsocket.cpp
    ruqolawebsocket.cpp
    messageattachment.cpp
    messageurl.cpp
    rocketchataccount.cpp
    rocketchataccountsettings.cpp
    ruqolalogger.cpp
    ruqolaregisterengine.cpp
    ruqolaserverconfig.cpp
    rocketchatcache.cpp
    texthighlighter.cpp
    textconverter.cpp
    loadrecenthistorymanager.cpp
    unityservicemanager.cpp
    file.cpp
    channel.cpp
    inputtextmanager.cpp
    authenticationmanager.cpp
    searchmessage.cpp

    accountmanager.cpp
    managerdatapaths.cpp

    notificationoptions.cpp

    reactions.cpp
    reaction.cpp
)

ecm_qt_declare_logging_category(Ruqola_core_srcs HEADER ruqola_debug.h IDENTIFIER RUQOLA_LOG CATEGORY_NAME org.kde.ruqola)
ecm_qt_declare_logging_category(Ruqola_core_srcs HEADER ruqola_message_debug.h IDENTIFIER RUQOLA_MESSAGE_LOG CATEGORY_NAME org.kde.ruqola.message)
ecm_qt_declare_logging_category(Ruqola_core_srcs HEADER ruqola_ddpapi_debug.h IDENTIFIER RUQOLA_DDPAPI_LOG CATEGORY_NAME org.kde.ruqola.ddpapi)
ecm_qt_declare_logging_category(Ruqola_core_srcs HEADER ruqola_ddpapi_command_debug.h IDENTIFIER RUQOLA_DDPAPI_COMMAND_LOG CATEGORY_NAME org.kde.ruqola.ddpapi.command)
ecm_qt_declare_logging_category(Ruqola_core_srcs HEADER ruqola_restapi_debug.h IDENTIFIER RUQOLA_RESTAPI_LOG CATEGORY_NAME org.kde.ruqola.restapi)

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
    Qt5::DBus
    KF5::CoreAddons
    KF5::I18n
    KF5::Crash
    KF5::Notifications
    KF5::KIOCore
    KF5::SyntaxHighlighting
)

if (WIN32 OR APPLE)
    target_link_libraries(libruqolacore KF5::IconThemes)
endif()

if (Qt5Keychain_FOUND)
    target_link_libraries(libruqolacore qt5keychain)
    target_include_directories(libruqolacore PRIVATE ${QTKEYCHAIN_INCLUDE_DIRS})
endif()

set_target_properties(libruqolacore
    PROPERTIES OUTPUT_NAME ruqolacore VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
    )

qt5_add_resources(RuqolaResources qml/qml.qrc)

add_executable(ruqola main.cpp ${RuqolaResources})

target_link_libraries(ruqola libruqolacore)

install(TARGETS ruqola ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(TARGETS libruqolacore ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

install(PROGRAMS data/org.kde.ruqola.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES data/org.kde.ruqola.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

add_subdirectory(plugins)
add_subdirectory(icons)
