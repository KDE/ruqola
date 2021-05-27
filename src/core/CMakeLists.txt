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
    model/notificationpreferencemodel.cpp

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

    model/accountschannelsmodel.cpp

    model/adminroomsmodel.cpp
    model/adminroomsfilterproxymodel.cpp

    model/adminusersmodel.cpp
    model/adminusersfilterproxymodel.cpp

    model/systemmessagesmodel.cpp
    model/channelcompletermodel.cpp
    model/channelcompleterfilterproxymodel.cpp

    model/teamroomsmodel.cpp
    model/teamroomsfilterproxymodel.cpp

    model/teamroomcompletermodel.cpp

    model/teamcompletermodel.cpp

    model/admininvitemodel.cpp
    model/admininvitefilterproxymodel.cpp

    model/directoryroomsmodel.cpp
    model/directoryteamsmodel.cpp
    model/directoryusersmodel.cpp
    model/directorybasemodel.cpp

    plugins/pluginauthenticationinterface.cpp
    plugins/pluginauthentication.cpp
    emoticons/customemoji.cpp
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

    autotranslatelanguage.cpp
    autotranslatelanguages.cpp

    command.cpp
    commands.cpp

    downloadappslanguages/downloadappslanguagesjob.cpp
    downloadappslanguages/downloadappslanguagesparser.cpp
    downloadappslanguages/downloadappslanguagesmanager.cpp
    downloadappslanguages/downloadappslanguagesinfo.cpp

    administrator/roominfo.cpp
    administrator/adminrooms.cpp

    authenticationinfo.cpp
    room.cpp
    ruqola.cpp
    ruqolautils.cpp
    rocketchatbackend.cpp
    messagequeue.cpp
    rocketchatmessage.cpp
    typingnotification.cpp
    changetemporarystatus.cpp
    user.cpp
    utils.cpp
    otr.cpp
    otrmanager.cpp
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
    file.cpp
    channel.cpp
    inputtextmanager.cpp
    authenticationmanager.cpp

    accountmanager.cpp
    managerdatapaths.cpp
    messagecache.cpp

    notificationoptions.cpp

    syntaxhighlightingmanager.cpp

    receivetypingnotificationmanager.cpp

    serverconfiginfo.cpp
    notificationpreferences.cpp

    roles.cpp
    role.cpp

    avatarmanager.cpp

    discussion.cpp
    discussions.cpp

    emoticoncategory.cpp

    listmessages.cpp

    messagedownloadmanager.cpp

    fileattachments.cpp
    commands.cpp

    accountroomsettings.cpp

    notifierjob.cpp
    restauthenticationmanager.cpp

    ownuser.cpp
    servicepassword.cpp

    downloadavartarurlmanager.cpp

    channelcounterinfo.cpp

    customuserstatuses.cpp
    customuserstatus.cpp

    ownuserpreferences.cpp

    permission.cpp
    permissionmanager.cpp

    serverinfo.cpp
    retentioninfo.cpp
    teaminfo.cpp
    teamroom.cpp
    teamroomcompleter.cpp
    teamcompleter.cpp

    inviteinfo.cpp
    ruqolacore.qrc
)

kconfig_add_kcfg_files(libruqolacore settings/ruqolaglobalconfig.kcfgc)


if (NOT WIN32)
    target_sources(libruqolacore PRIVATE unityservicemanager.cpp)
endif()

if (NOT ANDROID)
    target_sources(libruqolacore PRIVATE notification.cpp)
endif()

ecm_qt_declare_logging_category(libruqolacore HEADER ruqola_debug.h IDENTIFIER RUQOLA_LOG CATEGORY_NAME org.kde.ruqola DESCRIPTION "ruqola" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore HEADER ruqola_message_debug.h IDENTIFIER RUQOLA_MESSAGE_LOG CATEGORY_NAME org.kde.ruqola.message DESCRIPTION "ruqola (show message debug)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore HEADER ruqola_ddpapi_debug.h IDENTIFIER RUQOLA_DDPAPI_LOG CATEGORY_NAME org.kde.ruqola.ddpapi DESCRIPTION "ruqola (ddpapi)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore HEADER ruqola_ddpapi_command_debug.h IDENTIFIER RUQOLA_DDPAPI_COMMAND_LOG CATEGORY_NAME org.kde.ruqola.ddpapi.command DESCRIPTION "ruqola (ddpapi command)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore HEADER ruqola_unknown_collectiontype_debug.h IDENTIFIER RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG CATEGORY_NAME org.kde.ruqola.ddp.collectiontype DESCRIPTION "ruqola ddp unknown collection" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore HEADER ruqola_typing_notification_debug.h IDENTIFIER RUQOLA_TYPING_NOTIFICATION_LOG CATEGORY_NAME org.kde.ruqola.typingnotification DESCRIPTION "ruqola typingnotification" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore HEADER ruqola_completion_debug.h IDENTIFIER RUQOLA_COMPLETION_LOG CATEGORY_NAME org.kde.ruqola.completion DESCRIPTION "ruqola (completion)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore HEADER ruqola_notification_debug.h IDENTIFIER RUQOLA_NOTIFICATION_LOG CATEGORY_NAME org.kde.ruqola.notification DESCRIPTION "ruqola notification" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore HEADER ruqola_commands_debug.h IDENTIFIER RUQOLA_COMMANDS_LOG CATEGORY_NAME org.kde.ruqola.commands DESCRIPTION "ruqola command" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolacore HEADER ruqola_rooms_debug.h IDENTIFIER RUQOLA_ROOMS_LOG CATEGORY_NAME org.kde.ruqola.rooms DESCRIPTION "ruqola rooms" EXPORT RUQOLA)

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


if (NOT WIN32)
    target_link_libraries(libruqolacore Qt5::DBus)
endif()

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

