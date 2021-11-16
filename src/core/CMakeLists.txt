# SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_library(libruqolacore)

target_sources(libruqolacore PRIVATE
    ddpapi/ddpauthenticationmanager.cpp
    ddpapi/ddpclient.cpp
    ddpapi/ddpmanager.cpp
    ktexttohtmlfork/ruqolaktexttohtml.cpp
    model/messagemodel.cpp
    model/roommodel.cpp
    model/roomfilterproxymodel.cpp
    model/roomlistheadingsproxymodel.cpp

    model/usersforroommodel.cpp
    model/usersforroomfilterproxymodel.cpp

    model/usersmodel.cpp

    model/usercompleterfilterproxymodel.cpp
    model/usercompletermodel.cpp

    model/statusmodel.cpp
    model/statusmodelfilterproxymodel.cpp

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

    model/emoticonfiltermodel.cpp
    model/emoticonmodel.cpp
    model/emoticonmodelfilterproxymodel.cpp
    model/notificationpreferencemodel.cpp

    model/emoticoncustommodel.cpp
    model/emoticoncustommodelfilterproxymodel.cpp
    model/emoticoncategorymodelfilterproxymodel.cpp

    model/notificationdesktopdurationpreferencemodel.cpp
    model/notificationdesktopsoundpreferencemodel.cpp

    model/discussionsmodel.cpp
    model/discussionsfilterproxymodel.cpp

    model/emoticoncategoriesmodel.cpp

    model/threadmessagemodel.cpp

    model/listmessagesmodel.cpp
    model/listmessagesmodelfilterproxymodel.cpp

    model/autotranslatelanguagesmodel.cpp

    model/commandsmodel.cpp
    model/commandsmodelfilterproxymodel.cpp

    model/accountschannelsmodel.cpp

    model/adminroomsmodel.cpp
    model/adminroomsfilterproxymodel.cpp

    model/adminusersmodel.cpp

    model/systemmessagesmodel.cpp
    model/channelcompletermodel.cpp
    model/channelcompleterfilterproxymodel.cpp

    model/teamroomsmodel.cpp
    model/teamroomsfilterproxymodel.cpp

    model/teamroomcompletermodel.cpp

    model/teamcompletermodel.cpp

    model/admininvitemodel.cpp

    model/directoryroomsmodel.cpp
    model/directoryteamsmodel.cpp
    model/directoryusersmodel.cpp
    model/directorybasemodel.cpp
    model/directorybasefilterproxymodel.cpp
    model/directoryroomsproxymodel.cpp
    model/directoryteamsproxymodel.cpp
    model/directoryusersproxymodel.cpp

    model/rolesmodel.cpp
    model/admincustomsoundmodel.cpp
    model/admincustomemojimodel.cpp
    model/adminpermissionsmodel.cpp

    model/adminrolesmodel.cpp

    model/usersinrolemodel.cpp
    

    plugins/pluginauthenticationinterface.cpp
    plugins/pluginauthentication.cpp
    emoticons/customemoji.cpp
    emoticons/customemojisinfo.cpp
    emoticons/emojimanager.cpp
    emoticons/unicodeemoticon.cpp
    emoticons/unicodeemoticonparser.cpp
    emoticons/unicodeemoticonmanager.cpp
    messages/message.cpp
    messages/messageattachment.cpp
    messages/messageurl.cpp
    messages/messagestarred.cpp
    messages/reactions.cpp
    messages/reaction.cpp
    messages/messagepinned.cpp
    messages/messagetranslation.cpp
    messages/messageattachmentfield.cpp

    convertertextjob/convertertextabstractjob.cpp
    convertertextjob/translatetextjob.cpp

    otr/otr.cpp
    otr/otrmanager.cpp
    otr/otrnotificationjob.cpp

    autotranslate/autotranslatelanguage.cpp
    autotranslate/autotranslatelanguages.cpp

    commands/command.cpp
    commands/commands.cpp

    downloadappslanguages/downloadappslanguagesjob.cpp
    downloadappslanguages/downloadappslanguagesparser.cpp
    downloadappslanguages/downloadappslanguagesmanager.cpp
    downloadappslanguages/downloadappslanguagesinfo.cpp

    roominfo/roominfo.cpp
    roominfo/roomsinfo.cpp

    customsound/customsoundsinfo.cpp
    customsound/customsoundinfo.cpp
    customsound/customsoundsmanager.cpp

    users.cpp

    authenticationinfo.cpp
    room.cpp
    ruqola.cpp
    ruqolautils.cpp
    rocketchatbackend.cpp
    localmessagelogger.cpp
    messagequeue.cpp
    rocketchatmessage.cpp
    typingnotification.cpp
    changetemporarystatus.cpp
    user.cpp
    utils.cpp
    abstractwebsocket.cpp
    ruqolawebsocket.cpp
    rocketchataccount.cpp
    rocketchataccountsettings.cpp
    ruqolalogger.cpp
    ruqolaserverconfig.cpp
    rocketchatcache.cpp
    texthighlighter.cpp
    textconverter.cpp
    loadrecenthistorymanager.cpp
    channel.cpp
    inputtextmanager.cpp
    authenticationmanager.cpp

    accountmanager.cpp
    managerdatapaths.cpp
    messagecache.cpp

    notifications/notificationoptions.cpp

    syntaxhighlightingmanager.cpp

    receivetypingnotificationmanager.cpp

    serverconfiginfo.cpp
    notifications/notificationpreferences.cpp

    roles/roles.cpp
    roles/role.cpp
    roles/roleinfo.cpp
    roles/rolesmanager.cpp

    avatarmanager.cpp

    discussions/discussion.cpp
    discussions/discussions.cpp

    emoticons/emoticoncategory.cpp

    listmessages.cpp

    messagedownloadmanager.cpp

    attachments/fileattachments.cpp
    attachments/file.cpp

    accountroomsettings.cpp

    notifications/notifierjob.cpp
    restauthenticationmanager.cpp

    ownuser/ownuser.cpp
    servicepassword.cpp

    downloadavartarurlmanager.cpp

    channelcounterinfo.cpp

    customusers/customuserstatuses.cpp
    customusers/customuserstatus.cpp

    ownuser/ownuserpreferences.cpp

    permissions/permissions.cpp
    permissions/permission.cpp
    permissions/permissionmanager.cpp

    serverinfo.cpp
    retentioninfo.cpp
    teams/teaminfo.cpp
    teams/teamroom.cpp
    teams/teamroomcompleter.cpp
    teams/teamcompleter.cpp

    invite/inviteinfo.cpp

    ruqolacore.qrc
    managechannels.cpp
    texthighlighter.h
    avatarmanager.h
    utils.h
    listmessages.h
    authenticationinfo.h
    ddpapi/ddpclient.h
    ddpapi/ddpauthenticationmanager.h
    ddpapi/ddpmanager.h
    model/usersinrolemodel.h
    model/emoticoncategorymodelfilterproxymodel.h
    model/admincustomemojimodel.h
    model/emoticonfiltermodel.h
    model/statusmodel.h
    model/directoryroomsmodel.h
    model/directoryroomsproxymodel.h
    model/adminrolesmodel.h
    model/admininvitemodel.h
    model/notificationpreferencemodel.h
    model/notificationdesktopsoundpreferencemodel.h
    model/usercompleterfilterproxymodel.h
    model/usersforroommodel.h
    model/emoticoncustommodel.h
    model/usersmodel.h
    model/loginmethodmodel.h
    model/admincustomsoundmodel.h
    model/filesforroomfilterproxymodel.h
    model/adminroomsmodel.h
    model/searchchannelmodel.h
    model/emoticoncategoriesmodel.h
    model/channelcompleterfilterproxymodel.h
    model/adminroomsfilterproxymodel.h
    model/filesforroommodel.h
    model/usersforroomfilterproxymodel.h
    model/adminusersmodel.h
    model/directoryteamsmodel.h
    model/teamcompletermodel.h
    model/roomlistheadingsproxymodel.h
    model/notificationdesktopdurationpreferencemodel.h
    model/accountschannelsmodel.h
    model/directoryteamsproxymodel.h
    model/directoryusersproxymodel.h
    model/searchchannelfilterproxymodel.h
    model/rocketchataccountmodel.h
    model/autotranslatelanguagesmodel.h
    model/commandsmodelfilterproxymodel.h
    model/adminpermissionsmodel.h
    model/channelcompletermodel.h
    model/roomfilterproxymodel.h
    model/threadmessagemodel.h
    model/teamroomcompletermodel.h
    model/listmessagesmodelfilterproxymodel.h
    model/directoryusersmodel.h
    model/listmessagesmodel.h
    model/searchmessagefilterproxymodel.h
    model/rolesmodel.h
    model/commandsmodel.h
    model/searchmessagemodel.h
    model/emoticoncustommodelfilterproxymodel.h
    model/messagemodel.h
    model/directorybasefilterproxymodel.h
    model/usercompletermodel.h
    model/directorybasemodel.h
    model/systemmessagesmodel.h
    model/discussionsfilterproxymodel.h
    model/inputcompletermodel.h
    model/rocketchataccountfilterproxymodel.h
    model/discussionsmodel.h
    model/statusmodelfilterproxymodel.h
    model/roommodel.h
    model/emoticonmodelfilterproxymodel.h
    model/teamroomsmodel.h
    model/teamroomsfilterproxymodel.h
    model/emoticonmodel.h
    messages/reactions.h
    messages/messageattachment.h
    messages/reaction.h
    messages/messageurl.h
    messages/messagestarred.h
    messages/messagetranslation.h
    messages/message.h
    messages/messagepinned.h
    messages/messageattachmentfield.h
    ruqolautils.h
    libruqola_private_export.h
    serverconfiginfo.h
    managerdatapaths.h
    ruqolawebsocket.h
    rocketchataccount.h
    roominfo/roominfo.h
    roominfo/roomsinfo.h
    commands/commands.h
    commands/command.h
    ownuser/ownuserpreferences.h
    ownuser/ownuser.h
    convertertextjob/convertertextabstractjob.h
    convertertextjob/translatetextjob.h
    managechannels.h
    notifications/notification.h
    notifications/notifierjob.h
    notifications/notificationoptions.h
    notifications/notificationpreferences.h
    ruqolaserverconfig.h
    rocketchataccountsettings.h
    inputtextmanager.h
    room.h
    changetemporarystatus.h
    typingnotification.h
    autotranslate/autotranslatelanguage.h
    autotranslate/autotranslatelanguages.h
    rocketchatcache.h
    attachments/file.h
    attachments/fileattachments.h
    messagedownloadmanager.h
    accountmanager.h
    customusers/customuserstatuses.h
    customusers/customuserstatus.h
    ktexttohtmlfork/ruqolaktexttohtml_p.h
    ktexttohtmlfork/ruqolaktexttohtml.h
    ruqolalogger.h
    permissions/permissions.h
    permissions/permissionmanager.h
    permissions/permission.h
    otr/otrmanager.h
    otr/otr.h
    otr/otrnotificationjob.h
    loadrecenthistorymanager.h
    syntaxhighlightingmanager.h
    python/ruqolacore/ruqolacore_global.h
    serverinfo.h
    channelcounterinfo.h
    accountroomsettings.h
    customsound/customsoundinfo.h
    customsound/customsoundsinfo.h
    customsound/customsoundsmanager.h
    abstractwebsocket.h
    users.h
    ruqola.h
    downloadappslanguages/downloadappslanguagesinfo.h
    downloadappslanguages/downloadappslanguagesmanager.h
    downloadappslanguages/downloadappslanguagesjob.h
    downloadappslanguages/downloadappslanguagesparser.h
    discussions/discussion.h
    discussions/discussions.h
    plugins/pluginauthenticationinterface.h
    plugins/pluginauthentication.h
    receivetypingnotificationmanager.h
    servicepassword.h
    localmessagelogger.h
    user.h
    lrucache.h
    textconverter.h
    invite/inviteinfo.h
    retentioninfo.h
    restauthenticationmanager.h
    roles/roleinfo.h
    roles/rolesmanager.h
    roles/role.h
    roles/roles.h
    channel.h
    downloadavartarurlmanager.h
    teams/teaminfo.h
    teams/teamcompleter.h
    teams/teamroom.h
    teams/teamroomcompleter.h
    rocketchatmessage.h
    rocketchatbackend.h
    messagequeue.h
    authenticationmanager.h
    emoticons/unicodeemoticonmanager.h
    emoticons/emojimanager.h
    emoticons/customemoji.h
    emoticons/unicodeemoticon.h
    emoticons/emoticoncategory.h
    emoticons/unicodeemoticonparser.h
    emoticons/customemojisinfo.h
    messagecache.h
)

kconfig_add_kcfg_files(libruqolacore settings/ruqolaglobalconfig.kcfgc)


if (UNITY_SUPPORT)
    target_sources(libruqolacore PRIVATE unityservicemanager.cpp unityservicemanager.h)
    target_link_libraries(libruqolacore Qt5::DBus)
endif()

if (NOT ANDROID)
    target_sources(libruqolacore PRIVATE notifications/notification.cpp)
endif()

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_debug.h IDENTIFIER RUQOLA_LOG CATEGORY_NAME org.kde.ruqola DESCRIPTION "ruqola" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_message_debug.h IDENTIFIER RUQOLA_MESSAGE_LOG CATEGORY_NAME org.kde.ruqola.message DESCRIPTION "ruqola (show message debug)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_ddpapi_debug.h IDENTIFIER RUQOLA_DDPAPI_LOG CATEGORY_NAME org.kde.ruqola.ddpapi DESCRIPTION "ruqola (ddpapi)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_ddpapi_command_debug.h IDENTIFIER RUQOLA_DDPAPI_COMMAND_LOG CATEGORY_NAME org.kde.ruqola.ddpapi.command DESCRIPTION "ruqola (ddpapi command)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_unknown_collectiontype_debug.h IDENTIFIER RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG CATEGORY_NAME org.kde.ruqola.ddp.collectiontype DESCRIPTION "ruqola ddp unknown collection" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_typing_notification_debug.h IDENTIFIER RUQOLA_TYPING_NOTIFICATION_LOG CATEGORY_NAME org.kde.ruqola.typingnotification DESCRIPTION "ruqola typingnotification" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_completion_debug.h IDENTIFIER RUQOLA_COMPLETION_LOG CATEGORY_NAME org.kde.ruqola.completion DESCRIPTION "ruqola (completion)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_notification_debug.h IDENTIFIER RUQOLA_NOTIFICATION_LOG CATEGORY_NAME org.kde.ruqola.notification DESCRIPTION "ruqola notification" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_commands_debug.h IDENTIFIER RUQOLA_COMMANDS_LOG CATEGORY_NAME org.kde.ruqola.commands DESCRIPTION "ruqola command" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_rooms_debug.h IDENTIFIER RUQOLA_ROOMS_LOG CATEGORY_NAME org.kde.ruqola.rooms DESCRIPTION "ruqola rooms" EXPORT RUQOLA)
target_sources(libruqolacore PRIVATE ${libruqolacore_debug_SRCS})

ruqola_target_precompile_headers(libruqolacore PUBLIC ../../ruqola_pch.h)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(libruqolacore PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(libruqolacore BASE_NAME libruqolacore)

if (HAVE_SOLID)
  target_link_libraries(libruqolacore KF5::Solid)
endif()

if (HAVE_NETWORKMANAGER)
  target_link_libraries(libruqolacore KF5::NetworkManagerQt)
endif()

target_link_libraries(libruqolacore
    Qt5::Core
    Qt5::Gui
    Qt5::Sql
    Qt5::WebSockets
    Qt5::Network
    Qt5::NetworkAuth
    KF5::CoreAddons
    KF5::I18n
    KF5::Notifications
    KF5::SyntaxHighlighting
    librocketchatrestapi-qt5
    KF5::ConfigGui
    KF5::ConfigWidgets
)

if (WIN32 OR APPLE)
    target_link_libraries(libruqolacore KF5::IconThemes)
endif()

target_link_libraries(libruqolacore qt5keychain)
target_include_directories(libruqolacore PRIVATE ${QTKEYCHAIN_INCLUDE_DIRS})

set_target_properties(libruqolacore
    PROPERTIES OUTPUT_NAME ruqolacore VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
    )

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

install(TARGETS libruqolacore ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

if(OPTION_BUILD_PYTHON_BINDINGS)
    add_subdirectory(python)
endif()

