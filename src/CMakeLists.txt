
set (Ruqola_restapi_SRCS
    restapi/restapirequest.cpp
    restapi/restapiutil.cpp
    restapi/restapimethod.cpp
    restapi/uploadfilejob.cpp
    restapi/restapiabstractjob.cpp

    restapi/spotlightjob.cpp

    restapi/serverinfojob.cpp

    restapi/misc/owninfojob.cpp
    restapi/misc/settingsoauthjob.cpp

    restapi/users/getavatarjob.cpp
    restapi/users/setavatarjob.cpp
    restapi/users/forgotpasswordjob.cpp
    restapi/users/usersinfojob.cpp
    restapi/users/getpresencejob.cpp
    restapi/users/resetavatarjob.cpp
    restapi/users/getusernamesuggestionjob.cpp

    restapi/authentication/logoutjob.cpp
    restapi/authentication/loginjob.cpp
    restapi/authentication/facebookauthjob.cpp
    restapi/authentication/googleauthjob.cpp
    restapi/authentication/twitterauthjob.cpp

    restapi/settings/privateinfojob.cpp

    restapi/channellistjob.cpp
    restapi/downloadfilejob.cpp

    restapi/emoji/loademojicustomjob.cpp

    restapi/chat/starmessagejob.cpp
    restapi/chat/pinmessagejob.cpp
    restapi/chat/postmessagejob.cpp
    restapi/chat/deletemessagejob.cpp
    restapi/chat/updatemessagejob.cpp
    restapi/chat/reactonmessagejob.cpp
    restapi/chat/searchmessagejob.cpp
    restapi/chat/ignoreuserjob.cpp
    restapi/chat/reportmessagejob.cpp

    restapi/channels/changechanneltopicjob.cpp
    restapi/channels/changechannelannouncementjob.cpp
    restapi/channels/changechannelnamejob.cpp
    restapi/channels/createchanneljob.cpp
    restapi/channels/leavechanneljob.cpp
    restapi/channels/channelclosejob.cpp
    restapi/channels/channelhistoryjob.cpp
    restapi/channels/changechanneldescriptionjob.cpp
    restapi/channels/changechannelreadonlyjob.cpp
    restapi/channels/archivechanneljob.cpp
    restapi/channels/channelfilesjob.cpp
    restapi/channels/channelinvitejob.cpp
    restapi/channels/setjoincodechanneljob.cpp
    restapi/channels/channelremoveownerjob.cpp
    restapi/channels/setchanneltypejob.cpp
    restapi/channels/getchannelrolesjob.cpp

    restapi/groups/changegroupstopicjob.cpp
    restapi/groups/changegroupsannouncementjob.cpp
    restapi/groups/changegroupsnamejob.cpp
    restapi/groups/creategroupsjob.cpp
    restapi/groups/leavegroupsjob.cpp
    restapi/groups/changegroupsdescriptionjob.cpp
    restapi/groups/archivegroupsjob.cpp
    restapi/groups/groupsinvitejob.cpp
    restapi/groups/groupskickjob.cpp
    restapi/groups/setgrouptypejob.cpp
    restapi/groups/getgrouprolesjob.cpp

    restapi/directmessage/createdmjob.cpp

    restapi/rooms/savenotificationjob.cpp
    restapi/rooms/getroomsjob.cpp
    restapi/rooms/roomfavoritejob.cpp

    restapi/subscriptions/markroomasreadjob.cpp
    restapi/subscriptions/markroomasunreadjob.cpp

    restapi/permissions/listpermissionsjob.cpp
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
    notificationoptionswrapper.cpp
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
    syntaxhighlightingmanager.cpp
    copyfilejob.cpp
)

if (NOT WIN32)
    set(Ruqola_core_srcs ${Ruqola_core_srcs} unityservicemanager.cpp)
endif()


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
    KF5::CoreAddons
    KF5::I18n
    KF5::Crash
    KF5::Notifications
    KF5::SyntaxHighlighting
)

if (NOT WIN32)
    target_link_libraries(libruqolacore Qt5::DBus)
endif()

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
