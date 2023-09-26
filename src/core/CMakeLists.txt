# SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_library(libruqolacore)

target_sources(libruqolacore PRIVATE
    abstractwebsocket.cpp
    abstractwebsocket.h
    accountmanager.cpp
    accountmanager.h
    accountroomsettings.cpp
    accountroomsettings.h
    attachments/fileattachments.cpp
    attachments/fileattachments.h
    attachments/file.cpp
    attachments/file.h
    authenticationinfo.cpp
    authenticationinfo.h
    authenticationmanager.cpp
    authenticationmanager.h
    autotranslate/autotranslatelanguage.cpp
    autotranslate/autotranslatelanguage.h
    autotranslate/autotranslatelanguages.cpp
    autotranslate/autotranslatelanguages.h

    avatarmanager.cpp
    avatarmanager.h
    away/awaymanager.cpp
    away/awaymanager.h

    bannerinfo/bannerinfo.h
    bannerinfo/bannerinfo.cpp
    bannerinfo/bannerinfos.h
    bannerinfo/bannerinfos.cpp

    channelcounterinfo.cpp
    channelcounterinfo.h
    channel.cpp
    channel.h
    colors.cpp
    colors.h
    commands/command.cpp
    commands/command.h
    commands/commands.cpp
    commands/commands.h
    convertertextjob/convertertextabstractjob.cpp
    convertertextjob/convertertextabstractjob.h
    customsound/customsoundinfo.cpp
    customsound/customsoundinfo.h
    customsound/customsoundsinfo.cpp
    customsound/customsoundsinfo.h
    customsound/customsoundsmanager.cpp
    customsound/customsoundsmanager.h
    customusers/customuserstatus.cpp
    customusers/customuserstatuses.cpp
    customusers/customuserstatuses.h
    customusers/customuserstatus.h

    managedevices/deviceinfo.h
    managedevices/deviceinfo.cpp

    managedevices/deviceinfos.h
    managedevices/deviceinfos.cpp

    ddpapi/ddpauthenticationmanager.cpp
    ddpapi/ddpauthenticationmanager.h
    ddpapi/ddpclient.cpp
    ddpapi/ddpclient.h
    ddpapi/ddpmanager.cpp
    ddpapi/ddpmanager.h

    ddpapi/ddpauthenticationmanagerutils.h
    ddpapi/ddpauthenticationmanagerutils.cpp

    discussions/discussion.cpp
    discussions/discussion.h
    discussions/discussions.cpp
    discussions/discussions.h
    downloadappslanguages/downloadappslanguagesinfo.cpp
    downloadappslanguages/downloadappslanguagesinfo.h
    downloadappslanguages/downloadappslanguagesjob.cpp
    downloadappslanguages/downloadappslanguagesjob.h
    downloadappslanguages/downloadappslanguagesmanager.cpp
    downloadappslanguages/downloadappslanguagesmanager.h
    downloadappslanguages/downloadappslanguagesparser.cpp
    downloadappslanguages/downloadappslanguagesparser.h
    downloadavartarurlmanager.cpp
    downloadavartarurlmanager.h
    emoticons/customemoji.cpp
    emoticons/customemoji.h
    emoticons/customemojisinfo.cpp
    emoticons/customemojisinfo.h
    emoticons/emojimanager.cpp
    emoticons/emojimanager.h
    inputtextmanager.cpp
    inputtextmanager.h
    invite/inviteinfo.cpp
    invite/inviteinfo.h
    ktexttohtmlfork/ruqolaktexttohtml.cpp
    ktexttohtmlfork/ruqolaktexttohtml.h
    ktexttohtmlfork/ruqolaktexttohtml_p.h
    libruqola_private_export.h
    listmessages.cpp
    listmessages.h
    loadrecenthistorymanager.cpp
    loadrecenthistorymanager.h
    lrucache.h
    licenses/licensesmanager.h
    licenses/licensesmanager.cpp
    managechannels.cpp
    managechannels.h
    managerdatapaths.cpp
    managerdatapaths.h
    messagecache.cpp
    messagecache.h
    messagequeue.cpp
    messagequeue.h
    messages/messageattachment.cpp
    messages/messageattachmentfield.cpp
    messages/messageattachmentfield.h
    messages/messageattachment.h
    messages/message.cpp
    messages/message.h
    messages/messagepinned.cpp
    messages/messagepinned.h
    messages/messagestarred.cpp
    messages/messagestarred.h
    messages/messagetranslation.cpp
    messages/messagetranslation.h
    messages/messageurl.cpp
    messages/messageurl.h
    messages/reaction.cpp
    messages/reaction.h
    messages/reactions.cpp
    messages/reactions.h
    model/accountschannelsmodel.cpp
    model/accountschannelsmodel.h
    model/admincustomemojimodel.cpp
    model/admincustomemojimodel.h
    model/admincustomsoundmodel.cpp
    model/admincustomsoundmodel.h
    model/admininvitemodel.cpp
    model/admininvitemodel.h
    model/adminoauthmodel.cpp
    model/adminoauthmodel.h
    model/permissionsmodel.cpp
    model/permissionsmodel.h
    model/adminrolesmodel.cpp
    model/adminrolesmodel.h
    model/adminroomsfilterproxymodel.cpp
    model/adminroomsfilterproxymodel.h
    model/adminroomsmodel.cpp
    model/adminroomsmodel.h
    model/adminusersmodel.cpp
    model/adminusersmodel.h
    model/autotranslatelanguagesmodel.cpp
    model/autotranslatelanguagesmodel.h
    model/bannerinfosmodel.cpp
    model/bannerinfosmodel.h
    model/bannerinfosfilterproxymodel.h
    model/bannerinfosfilterproxymodel.cpp
    model/channelcompleterfilterproxymodel.cpp
    model/channelcompleterfilterproxymodel.h
    model/channelcompletermodel.cpp
    model/channelcompletermodel.h
    model/commandsmodel.cpp
    model/commandsmodelfilterproxymodel.cpp
    model/commandsmodelfilterproxymodel.h
    model/commandsmodel.h
    model/deviceinfomodel.cpp
    model/deviceinfomodel.h
    model/searchtreebasefilterproxymodel.cpp
    model/searchtreebasefilterproxymodel.h
    model/custombasemodel.cpp
    model/custombasemodel.h
    model/directoryroomsmodel.cpp
    model/directoryroomsmodel.h
    model/directoryroomsproxymodel.cpp
    model/directoryroomsproxymodel.h
    model/directoryteamsmodel.cpp
    model/directoryteamsmodel.h
    model/directoryteamsproxymodel.cpp
    model/directoryteamsproxymodel.h
    model/directoryusersmodel.cpp
    model/directoryusersmodel.h
    model/directoryusersproxymodel.cpp
    model/directoryusersproxymodel.h
    model/discussionsfilterproxymodel.cpp
    model/discussionsfilterproxymodel.h
    model/discussionsmodel.cpp
    model/discussionsmodel.h
    model/emoticonmodel.cpp
    model/emoticonmodelfilterproxymodel.cpp
    model/emoticonmodelfilterproxymodel.h
    model/emoticonmodel.h
    model/filesforroomfilterproxymodel.cpp
    model/filesforroomfilterproxymodel.h
    model/filesforroommodel.cpp
    model/filesforroommodel.h
    model/inputcompletermodel.cpp
    model/inputcompletermodel.h
    model/listmessagesmodel.cpp
    model/listmessagesmodelfilterproxymodel.cpp
    model/listmessagesmodelfilterproxymodel.h
    model/listmessagesmodel.h
    model/loginmethodmodel.cpp
    model/loginmethodmodel.h
    model/messagesmodel.cpp
    model/messagesmodel.h
    model/notificationdesktopdurationpreferencemodel.cpp
    model/notificationdesktopdurationpreferencemodel.h
    model/notificationdesktopsoundpreferencemodel.cpp
    model/notificationdesktopsoundpreferencemodel.h
    model/notificationhistorymodel.cpp
    model/notificationhistorymodel.h
    model/notificationhistorymodelfilterproxymodel.h
    model/notificationhistorymodelfilterproxymodel.cpp
    model/notificationpreferencemodel.cpp
    model/notificationpreferencemodel.h
    model/personalaccesstokeninfosmodel.cpp
    model/personalaccesstokeninfosmodel.h
    model/personalaccesstokeninfosfilterproxymodel.cpp
    model/personalaccesstokeninfosfilterproxymodel.h
    model/rocketchataccountfilterproxymodel.cpp
    model/rocketchataccountfilterproxymodel.h
    model/rocketchataccountmodel.cpp
    model/rocketchataccountmodel.h
    model/rolesmodel.cpp
    model/rolesmodel.h
    model/roomfilterproxymodel.cpp
    model/roomfilterproxymodel.h
    model/roomlistheadingsproxymodel.cpp
    model/roomlistheadingsproxymodel.h
    model/roommodel.cpp
    model/roommodel.h
    model/searchchannelfilterproxymodel.cpp
    model/searchchannelfilterproxymodel.h
    model/searchchannelmodel.cpp
    model/searchchannelmodel.h
    model/searchmessagefilterproxymodel.cpp
    model/searchmessagefilterproxymodel.h
    model/commonmessagemodel.cpp
    model/commonmessagemodel.h
    model/statusmodel.cpp
    model/statusmodelfilterproxymodel.cpp
    model/statusmodelfilterproxymodel.h
    model/statusmodel.h
    model/switchchannelhistorymodel.cpp
    model/switchchannelhistorymodel.h
    model/systemmessagesmodel.cpp
    model/systemmessagesmodel.h
    model/teamcompletermodel.cpp
    model/teamcompletermodel.h
    model/teamroomcompletermodel.cpp
    model/teamroomcompletermodel.h
    model/teamroomsfilterproxymodel.cpp
    model/teamroomsfilterproxymodel.h
    model/teamroomsmodel.cpp
    model/teamroomsmodel.h
    model/threadmessagemodel.cpp
    model/threadmessagemodel.h
    model/usercompleterfilterproxymodel.cpp
    model/usercompleterfilterproxymodel.h
    model/usercompletermodel.cpp
    model/usercompletermodel.h
    model/usersforroomfilterproxymodel.cpp
    model/usersforroomfilterproxymodel.h
    model/usersforroommodel.cpp
    model/usersforroommodel.h
    model/usersinrolemodel.cpp
    model/usersinrolemodel.h
    model/usersmodel.cpp
    model/usersmodel.h
    model/servererrorinfohistorymodel.h
    model/servererrorinfohistorymodel.cpp
    model/servererrorinfohistoryfilterproxymodel.cpp
    model/servererrorinfohistoryfilterproxymodel.h
    model/moderationmodel.cpp
    model/moderationmodel.h


    notificationinfo.cpp
    notificationinfo.h
    notifications/notification.cpp
    notifications/notification.h
    notifications/notificationoptions.cpp
    notifications/notificationoptions.h
    notifications/notificationpreferences.cpp
    notifications/notificationpreferences.h
    notifications/notifierjob.cpp
    notifications/notifierjob.h
    notificationhistorymanager.cpp
    servererrorinfohistorymanager.cpp
    notificationhistorymanager.h
    servererrorinfohistorymanager.h
    oauth/oauthinfo.cpp
    oauth/oauthinfo.h
    otr/otr.cpp
    otr/otr.h
    otr/otrmanager.cpp
    otr/otrmanager.h
    otr/otrnotificationjob.cpp
    otr/otrnotificationjob.h
    ownuser/ownuser.cpp
    ownuser/ownuser.h
    ownuser/ownuserpreferences.cpp
    ownuser/ownuserpreferences.h
    parsemessageurlutils.cpp
    parsemessageurlutils.h
    permissions/permission.cpp
    permissions/permission.h
    permissions/permissionmanager.cpp
    permissions/permissionmanager.h
    permissions/permissions.cpp
    permissions/permissions.h

    personalaccesstokens/personalaccesstokeninfo.h
    personalaccesstokens/personalaccesstokeninfo.cpp
    personalaccesstokens/personalaccesstokeninfos.h
    personalaccesstokens/personalaccesstokeninfos.cpp

    plugins/pluginauthentication.cpp
    plugins/pluginauthentication.h
    plugins/pluginauthenticationinterface.cpp
    plugins/pluginauthenticationinterface.h
    python/ruqolacore/ruqolacore_global.h
    receivetypingnotificationmanager.cpp
    receivetypingnotificationmanager.h
    restauthenticationmanager.cpp
    restauthenticationmanager.h
    retentioninfo.cpp
    retentioninfo.h
    rocketchataccount.cpp
    rocketchataccount.h
    rocketchataccountsettings.cpp
    rocketchataccountsettings.h
    rocketchatbackend.cpp
    rocketchatbackend.h
    rocketchatcache.cpp
    rocketchatcache.h
    rocketchatmessage.cpp
    rocketchatmessage.h
    roles/role.cpp
    roles/role.h
    roles/roleinfo.cpp
    roles/roleinfo.h
    roles/roles.cpp
    roles/roles.h
    roles/rolesmanager.cpp
    roles/rolesmanager.h
    room.cpp
    room.h
    roominfo/roominfo.cpp
    roominfo/roominfo.h
    roominfo/roomsinfo.cpp
    roominfo/roomsinfo.h
    ruqola.cpp
    ruqola.h
    ruqolalogger.cpp
    ruqolalogger.h
    ruqolaserverconfig.cpp
    ruqolaserverconfig.h
    ruqolautils.cpp
    ruqolautils.h
    ruqolawebsocket.cpp
    ruqolawebsocket.h
    serverconfiginfo.cpp
    serverconfiginfo.h
    serverinfo.cpp
    serverinfo.h
    servicepassword.cpp
    servicepassword.h
    syntaxhighlightingmanager.cpp
    syntaxhighlightingmanager.h
    teams/teamcompleter.cpp
    teams/teamcompleter.h
    teams/teaminfo.cpp
    teams/teaminfo.h
    teams/teamroomcompleter.cpp
    teams/teamroomcompleter.h
    teams/teamroom.cpp
    teams/teamroom.h
    textconverter.cpp
    textconverter.h
    texthighlighter.cpp
    texthighlighter.h
    typingnotification.cpp
    typingnotification.h
    uploadfilemanager.cpp
    uploadfilemanager.h
    user.cpp
    user.h
    users.cpp
    users.h
    utils.cpp
    utils.h

    ruqolacore.qrc

    messages/block.h
    messages/block.cpp

    videoconference/videoconferencemanager.h
    videoconference/videoconferencemanager.cpp

    videoconference/videoconferencenotificationjob.h
    videoconference/videoconferencenotificationjob.cpp
    videoconference/videoconference.h
    videoconference/videoconference.cpp
    videoconference/videoconferenceinfo.h
    videoconference/videoconferenceinfo.cpp
    videoconference/updatevideoconferencemessagejob.h
    videoconference/updatevideoconferencemessagejob.cpp
    videoconference/videoconferencemessageinfomanager.h
    videoconference/videoconferencemessageinfomanager.cpp

    servererrorinfo.h
    servererrorinfo.cpp
    servererrorinfohistorymanager.cpp
    servererrorinfohistorymanager.h

    localdatabase/localmessagelogger.cpp
    localdatabase/localmessagelogger.h

    localdatabase/localdatabasemanager.h
    localdatabase/localdatabasemanager.cpp

    localdatabase/localmessagedatabase.h
    localdatabase/localmessagedatabase.cpp

    localdatabase/localdatabaseutils.h
    localdatabase/localdatabaseutils.cpp

    localdatabase/localroomsdatabase.h
    localdatabase/localroomsdatabase.cpp

    localdatabase/localdatabasebase.h
    localdatabase/localdatabasebase.cpp

    localdatabase/localaccountdatabase.h
    localdatabase/localaccountdatabase.cpp

    localdatabase/globaldatabase.h
    localdatabase/globaldatabase.cpp

    customemojiiconmanager.h
    customemojiiconmanager.cpp

    managelocaldatabase.h
    managelocaldatabase.cpp

    manageloadhistoryparsesyncmessagesutils.h
    manageloadhistoryparsesyncmessagesutils.cpp

    moderation/moderationinfo.h
    moderation/moderationinfo.cpp
    moderation/moderationinfos.cpp
    moderation/moderationinfos.h

    moderation/moderationmessageinfo.h
    moderation/moderationmessageinfo.cpp
)

if (HAVE_TEXT_TRANSLATOR)
    target_sources(libruqolacore PRIVATE
        translatetext/translatetextjob.cpp
        translatetext/translatetextjob.h
        translatetext/translatorenginemanager.cpp
        translatetext/translatorenginemanager.h
    )
endif()

kconfig_add_kcfg_files(libruqolacore settings/ruqolaglobalconfig.kcfgc)


if (UNITY_SUPPORT)
    target_sources(libruqolacore PRIVATE unityservicemanager.cpp unityservicemanager.h)
    target_link_libraries(libruqolacore Qt::DBus)
endif()


ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_debug.h IDENTIFIER RUQOLA_LOG
    CATEGORY_NAME org.kde.ruqola DESCRIPTION "ruqola" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_message_debug.h IDENTIFIER RUQOLA_MESSAGE_LOG
    CATEGORY_NAME org.kde.ruqola.message DESCRIPTION "ruqola (show message debug)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_ddpapi_debug.h IDENTIFIER RUQOLA_DDPAPI_LOG
    CATEGORY_NAME org.kde.ruqola.ddpapi DESCRIPTION "ruqola (ddpapi)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_ddpapi_command_debug.h IDENTIFIER RUQOLA_DDPAPI_COMMAND_LOG
    CATEGORY_NAME org.kde.ruqola.ddpapi.command DESCRIPTION "ruqola (ddpapi command)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_unknown_collectiontype_debug.h IDENTIFIER RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG
    CATEGORY_NAME org.kde.ruqola.ddp.collectiontype DESCRIPTION "ruqola ddp unknown collection" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_typing_notification_debug.h IDENTIFIER RUQOLA_TYPING_NOTIFICATION_LOG
    CATEGORY_NAME org.kde.ruqola.typingnotification DESCRIPTION "ruqola typingnotification" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_completion_debug.h IDENTIFIER RUQOLA_COMPLETION_LOG
    CATEGORY_NAME org.kde.ruqola.completion DESCRIPTION "ruqola (completion)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_notification_debug.h IDENTIFIER RUQOLA_NOTIFICATION_LOG
    CATEGORY_NAME org.kde.ruqola.notification DESCRIPTION "ruqola notification" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_commands_debug.h IDENTIFIER RUQOLA_COMMANDS_LOG
    CATEGORY_NAME org.kde.ruqola.commands DESCRIPTION "ruqola command" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_rooms_debug.h IDENTIFIER RUQOLA_ROOMS_LOG
    CATEGORY_NAME org.kde.ruqola.rooms DESCRIPTION "ruqola rooms" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_custom_sounds_debug.h IDENTIFIER RUQOLA_CUSTOMSOUNDS_LOG
    CATEGORY_NAME org.kde.ruqola.customsounds DESCRIPTION "ruqola custom sounds" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_away_debug.h IDENTIFIER RUQOLA_AWAY_LOG
    CATEGORY_NAME org.kde.ruqola.away DESCRIPTION "ruqola away manager" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_texttohtml_debug.h IDENTIFIER RUQOLA_TEXTTOHTML_LOG
    CATEGORY_NAME org.kde.ruqola.texttohtml DESCRIPTION "ruqola text to html support" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_specialwarning_debug.h IDENTIFIER RUQOLA_SPECIALWARNING_LOG
    CATEGORY_NAME org.kde.ruqola.specialwarning DESCRIPTION "ruqola special warning" EXPORT RUQOLA)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_thread_message_debug.h IDENTIFIER RUQOLA_THREAD_MESSAGE_LOG
    CATEGORY_NAME org.kde.ruqola.threadmessages DESCRIPTION "ruqola thread message" EXPORT RUQOLA)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_translation_debug.h IDENTIFIER RUQOLA_TRANSLATION_LOG
    CATEGORY_NAME org.kde.ruqola.translation DESCRIPTION "ruqola translation" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_password_core_debug.h IDENTIFIER RUQOLA_PASSWORD_CORE_LOG
    CATEGORY_NAME org.kde.ruqola.password DESCRIPTION "ruqola password support" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_reconnect_core_debug.h IDENTIFIER RUQOLA_RECONNECT_LOG
    CATEGORY_NAME org.kde.ruqola.reconnect DESCRIPTION "ruqola reconnect support" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_videoconference_core_debug.h IDENTIFIER RUQOLA_VIDEO_CONFERENCE_LOG
    CATEGORY_NAME org.kde.ruqola.videoconference DESCRIPTION "ruqola video conference support" EXPORT RUQOLA)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_database_debug.h IDENTIFIER RUQOLA_DATABASE_LOG
    CATEGORY_NAME org.kde.ruqola.database DESCRIPTION "ruqola database support" EXPORT RUQOLA)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_loadhistory_debug.h IDENTIFIER RUQOLA_LOAD_HISTORY_LOG
    CATEGORY_NAME org.kde.ruqola.loadhistory DESCRIPTION "ruqola load history" EXPORT RUQOLA)

target_sources(libruqolacore PRIVATE ${libruqolacore_debug_SRCS})

ruqola_target_precompile_headers(libruqolacore PUBLIC ../../ruqola_pch.h)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(libruqolacore PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(libruqolacore BASE_NAME libruqolacore)

if (HAVE_SOLID)
    target_link_libraries(libruqolacore KF${KF_MAJOR_VERSION}::Solid)
endif()

if (HAVE_NETWORKMANAGER)
    target_link_libraries(libruqolacore KF${KF_MAJOR_VERSION}::NetworkManagerQt)
endif()

target_link_libraries(libruqolacore
    Qt::Core
    Qt::Gui
    Qt::Sql
    Qt::WebSockets
    Qt::Network
    Qt::NetworkAuth
    KF${KF_MAJOR_VERSION}::CoreAddons
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::Notifications
    KF${KF_MAJOR_VERSION}::SyntaxHighlighting
    librocketchatrestapi-qt5
    KF${KF_MAJOR_VERSION}::ConfigGui
    KF${KF_MAJOR_VERSION}::ConfigWidgets
    KF${KF_MAJOR_VERSION}::IdleTime
    KF${KF_MAJOR_VERSION}::WindowSystem
)

if (HAVE_TEXT_TRANSLATOR)
    target_link_libraries(libruqolacore
        KF${KF_MAJOR_VERSION}::TextTranslator
    )
endif()

if (HAVE_TEXT_AUTOCORRECTION)
    target_link_libraries(libruqolacore
        KF${KF_MAJOR_VERSION}::TextAutoCorrection
    )
endif()
if (HAVE_TEXT_AUTOCORRECTION_WIDGETS)
    target_link_libraries(libruqolacore
        KF${KF_MAJOR_VERSION}::TextAutoCorrectionWidgets
    )
endif()

if (HAVE_TEXT_UTILS)
    target_link_libraries(libruqolacore
        KF${KF_MAJOR_VERSION}::TextUtils
    )
endif()

if (WIN32 OR APPLE)
    target_link_libraries(libruqolacore KF${KF_MAJOR_VERSION}::IconThemes)
endif()
if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(libruqolacore qt6keychain)
else()
    target_link_libraries(libruqolacore qt5keychain)
endif()
target_include_directories(libruqolacore PRIVATE ${QTKEYCHAIN_INCLUDE_DIRS})

if (TARGET KF${KF_MAJOR_VERSION}::TextEmoticonsCore)
    target_link_libraries(libruqolacore KF${KF_MAJOR_VERSION}::TextEmoticonsCore)
endif()

if (QT_MAJOR_VERSION STREQUAL "6")
    target_link_libraries(libruqolacore KF6::StatusNotifierItem)
endif()

set_target_properties(libruqolacore
    PROPERTIES OUTPUT_NAME ruqolacore VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
)

if (BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(localdatabase/autotests)
endif()

install(TARGETS libruqolacore ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

if(OPTION_BUILD_PYTHON_BINDINGS)
    add_subdirectory(python)
endif()

