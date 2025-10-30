# SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_library(libruqolacore)

target_sources(
    libruqolacore
    PRIVATE
        abstractwebsocket.cpp
        abstractwebsocket.h
        connection.cpp
        connection.h
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
        colorsandmessageviewstyle.cpp
        colorsandmessageviewstyle.h
        commands/command.cpp
        commands/command.h
        commands/commands.cpp
        commands/commands.h
        commands/previewcommand.h
        commands/previewcommand.cpp
        commands/previewcommandutils.h
        commands/previewcommandutils.cpp
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
        channelusercompleter.h
        channelusercompleter.cpp
        managedevices/deviceinfo.h
        managedevices/deviceinfo.cpp
        managedevices/deviceinfos.h
        managedevices/deviceinfos.cpp
        ddpapi/ddpclient.cpp
        ddpapi/ddpclient.h
        ddpapi/ddpmanager.cpp
        ddpapi/ddpmanager.h
        discussions/discussion.cpp
        discussions/discussion.h
        discussions/discussions.cpp
        discussions/discussions.h
        downloadappslanguages/downloadappslanguagesinfo.cpp
        downloadappslanguages/downloadappslanguagesinfo.h
        downloadappslanguages/downloadappslanguagesmanager.cpp
        downloadappslanguages/downloadappslanguagesmanager.h
        downloadappslanguages/downloadappslanguagesparser.cpp
        downloadappslanguages/downloadappslanguagesparser.h
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
        messages/systemmessagetypeutil.h
        messages/systemmessagetypeutil.cpp
        messages/messageextra.h
        messages/messageextra.cpp
        messages/messagepinned.cpp
        messages/messagepinned.h
        messages/messagestarred.cpp
        messages/messagestarred.h
        messages/messagetranslations.cpp
        messages/messagetranslations.h
        messages/messageurl.cpp
        messages/messageurl.h
        messages/reaction.cpp
        messages/reaction.h
        messages/reactions.cpp
        messages/reactions.h
        messages/channels.cpp
        messages/channels.h
        messages/block/blocks.cpp
        messages/block/blocks.h
        messages/block/blockaccessory.h
        messages/block/blockaccessory.cpp
        messages/block/blockaccessoryoption.h
        messages/block/blockaccessoryoption.cpp
        messages/block/block.h
        messages/block/block.cpp
        messages/block/blockaction.h
        messages/block/blockaction.cpp
        messages/messageurls.h
        messages/messageurls.cpp
        messages/messageattachments.cpp
        messages/messageattachments.h
        messages/messageutils.h
        messages/messageutils.cpp
        messages/replies.h
        messages/replies.cpp
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
        model/administratoroauthfilterproxymodel.cpp
        model/administratoroauthfilterproxymodel.h
        model/permissionsmodel.cpp
        model/permissionsmodel.h
        model/adminrolesmodel.cpp
        model/adminrolesmodel.h
        model/adminroomsfilterproxymodel.cpp
        model/adminroomsfilterproxymodel.h
        model/adminroomsmodel.cpp
        model/adminroomsmodel.h
        model/adminusersallmodel.cpp
        model/adminusersallmodel.h
        model/adminusersstatusmodel.h
        model/adminusersstatusmodel.cpp
        model/adminuserspendingmodel.cpp
        model/adminuserspendingmodel.h
        model/adminusersbasemodel.h
        model/adminusersbasemodel.cpp
        model/autotranslatelanguagesmodel.cpp
        model/autotranslatelanguagesmodel.h
        model/bannerinfosmodel.cpp
        model/bannerinfosmodel.h
        model/bannerinfosfilterproxymodel.h
        model/bannerinfosfilterproxymodel.cpp
        model/channelcompleterfilterproxymodel.cpp
        model/channelcompleterfilterproxymodel.h
        model/commandsmodel.cpp
        model/commandsmodelfilterproxymodel.cpp
        model/commandsmodelfilterproxymodel.h
        model/commandsmodel.h
        model/deviceinfomodel.cpp
        model/deviceinfomodel.h
        model/deviceinfoproxymodel.h
        model/deviceinfoproxymodel.cpp
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
        model/emoticonfilterproxymodel.cpp
        model/emoticonfilterproxymodel.h
        model/emoticonmodel.h
        model/filesforroomfilterproxymodel.cpp
        model/filesforroomfilterproxymodel.h
        model/filesforroommodel.cpp
        model/filesforroommodel.h
        model/inputcompletermodel.cpp
        model/inputcompletermodel.h
        model/listmessagesmodel.cpp
        model/listmessagesfilterproxymodel.cpp
        model/listmessagesfilterproxymodel.h
        model/listmessagesmodel.h
        model/messagesmodel.cpp
        model/messagesmodel.h
        model/notificationdesktopdurationpreferencemodel.cpp
        model/notificationdesktopdurationpreferencemodel.h
        model/notificationdesktopsoundpreferencemodel.cpp
        model/notificationdesktopsoundpreferencemodel.h
        model/notificationdesktopsoundpreferenceproxymodel.h
        model/notificationdesktopsoundpreferenceproxymodel.cpp
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
        model/commonmessagefilterproxymodel.cpp
        model/commonmessagefilterproxymodel.h
        model/commonmessagesmodel.cpp
        model/commonmessagesmodel.h
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
        model/moderationreportedmessagemodel.cpp
        model/moderationreportedmessagemodel.h
        model/moderationreportedmessageproxymodel.cpp
        model/moderationreportedmessageproxymodel.h
        model/moderationreportedusermodel.cpp
        model/moderationreportedusermodel.h
        model/moderationreporteduserproxymodel.cpp
        model/moderationreporteduserproxymodel.h
        model/moderationmessagesmodel.h
        model/moderationmessagesmodel.cpp
        model/moderationreportinfomodel.cpp
        model/moderationreportinfomodel.h
        model/moderationreportinfofilterproxymodel.cpp
        model/moderationreportinfofilterproxymodel.h
        model/sortfilterproxymodelbase.h
        model/sortfilterproxymodelbase.cpp
        model/joinedchannelmodel.h
        model/joinedchannelmodel.cpp
        model/videoconferencesmodel.cpp
        model/videoconferencesmodel.h
        model/appscategoriesmodel.cpp
        model/appscategoriesmodel.h
        model/appsmarketplacemodel.cpp
        model/appsmarketplacemodel.h
        model/appsmarketplacefilterproxymodel.h
        model/appsmarketplacefilterproxymodel.cpp
        model/previewcommandmodel.h
        model/previewcommandmodel.cpp
        model/usersforroomlistheadingsproxymodel.h
        model/usersforroomlistheadingsproxymodel.cpp
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
        parserocketchaturlutils.cpp
        parserocketchaturlutils.h
        rocketchaturlutils.h
        rocketchaturlutils.cpp
        permissions/permission.cpp
        permissions/permission.h
        permissions/permissionsmanager.cpp
        permissions/permissionsmanager.h
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
        plugins/pluginauthenticationconfigurewidget.h
        plugins/pluginauthenticationconfigurewidget.cpp
        authenticationmanager/restauthenticationmanager.cpp
        authenticationmanager/restauthenticationmanager.h
        authenticationmanager/ddpauthenticationmanager.cpp
        authenticationmanager/ddpauthenticationmanager.h
        authenticationmanager/authenticationmanagerutils.h
        authenticationmanager/authenticationmanagerutils.cpp
        authenticationmanager/authenticationmanagerbase.h
        authenticationmanager/authenticationmanagerbase.cpp
        receivetypingnotificationmanager.cpp
        receivetypingnotificationmanager.h
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
        roomextra.h
        roomextra.cpp
        roomencryptionkey.h
        roomencryptionkey.cpp
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
        servicepassword.cpp
        servicepassword.h
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
        videoconference/videoconferencemanager.h
        videoconference/videoconferencemanager.cpp
        videoconference/videoconferencenotificationjob.h
        videoconference/videoconferencenotificationjob.cpp
        videoconference/videoconference.h
        videoconference/videoconference.cpp
        videoconference/videoconferenceinfo.h
        videoconference/videoconferenceinfo.cpp
        videoconference/videoconferenceinfos.cpp
        videoconference/videoconferenceinfos.h
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
        localdatabase/localmessagesdatabase.h
        localdatabase/localmessagesdatabase.cpp
        localdatabase/localdatabaseutils.h
        localdatabase/localdatabaseutils.cpp
        localdatabase/localroomsdatabase.h
        localdatabase/localroomsdatabase.cpp
        localdatabase/localdatabasebase.h
        localdatabase/localdatabasebase.cpp
        localdatabase/localaccountsdatabase.h
        localdatabase/localaccountsdatabase.cpp
        localdatabase/e2edatabase.h
        localdatabase/e2edatabase.cpp
        localdatabase/globaldatabase.h
        localdatabase/globaldatabase.cpp
        customemojiiconmanager.h
        customemojiiconmanager.cpp
        managelocaldatabase.h
        managelocaldatabase.cpp
        manageloadhistoryparsesyncmessagesutils.h
        manageloadhistoryparsesyncmessagesutils.cpp
        moderation/moderationreportedmessageinfo.h
        moderation/moderationreportedmessageinfo.cpp
        moderation/moderationreportedmessageinfos.cpp
        moderation/moderationreportedmessageinfos.h
        moderation/moderationreportinfo.h
        moderation/moderationreportinfo.cpp
        moderation/moderationreportinfos.cpp
        moderation/moderationreportinfos.h
        moderation/moderationmessage.cpp
        moderation/moderationmessage.h
        moderation/moderationlistmessages.h
        moderation/moderationlistmessages.cpp
        moderation/moderationreporteduserinfo.h
        moderation/moderationreporteduserinfo.cpp
        moderation/moderationreporteduserinfos.cpp
        moderation/moderationreporteduserinfos.h
        moderation/moderationreportuserinfo.h
        moderation/moderationreportuserinfo.cpp
        moderation/moderationreportuserinfos.h
        moderation/moderationreportuserinfos.cpp
        previewurlcachemanager.h
        previewurlcachemanager.cpp
        encryption/e2ekeymanager.cpp
        encryption/e2ekeymanager.h
        encryption/e2ekey.h
        encryption/e2ekey.cpp
        soundmanager.h
        soundmanager.cpp
        apps/appscategoryinfo.h
        apps/appscategoryinfo.cpp
        apps/appsmarketplaceinfo.h
        apps/appsmarketplaceinfo.cpp
        apps/appscountinfo.h
        apps/appscountinfo.cpp
        apps/appsmarketplaceinstalledinfo.h
        apps/appsmarketplaceinstalledinfo.cpp
        apps/applicationssettingssettingsinfo.h
        apps/applicationssettingssettingsinfo.cpp
        apps/applicationssettingslogsinfo.h
        apps/applicationssettingslogsinfo.cpp
        memorymanager/memorymanager.h
        memorymanager/memorymanager.cpp
        job/extractserverinfojob.h
        job/extractserverinfojob.cpp
        job/validateinviteserverjob.h
        job/validateinviteserverjob.cpp
        actionbuttons/actionbutton.h
        actionbuttons/actionbutton.cpp
        actionbuttons/actionbuttonsmanager.h
        actionbuttons/actionbuttonsmanager.cpp
        actionbuttons/actionbuttonutil.h
        actionbuttons/actionbuttonutil.cpp

        ruqolacommandlineparser.cpp
        ruqolacommandlineparser.h
        quicksearch/ruqolaquicksearchmessagesettings.cpp
        quicksearch/ruqolaquicksearchmessagesettings.h
        emoji.qrc
)

if (HAVE_TEXTTOSPEECH_ENQUEUE_SUPPORT)
    target_sources(
        libruqolacore
        PRIVATE
        texttospeech/texttospeechenqueueinfo.h
        texttospeech/texttospeechenqueueinfo.cpp
    )
endif()

if (NOT HAVE_TEXTUTILS_SYNTAXHIGHLIGTHER_SUPPORT)
    target_sources(
        libruqolacore
        PRIVATE
        syntaxhighlightingmanager.cpp
        syntaxhighlightingmanager.h
        texthighlighter.cpp
        texthighlighter.h
    )
endif()

if(USE_E2E_SUPPORT)
    target_sources(
        libruqolacore
        PRIVATE
        encryption/encryptionutils.h
        encryption/encryptionutils.cpp
    )
endif()

if (HAVE_TEXTUTILS_CMARK_SUPPORT)
    target_sources(
        libruqolacore
        PRIVATE
        ruqolablockcmarksupport.h
        ruqolablockcmarksupport.cpp
    )
endif()

if(HAVE_TEXT_TRANSLATOR)
    target_sources(
        libruqolacore
        PRIVATE
            translatetext/translatetextjob.cpp
            translatetext/translatetextjob.h
            translatetext/translatorenginemanager.cpp
            translatetext/translatorenginemanager.h
    )
endif()

if(TARGET KF6::TextAutoGenerateText)
    target_link_libraries(libruqolacore KF6::TextAutoGenerateText)
endif()

if(TARGET Plasma::Activities)
    target_sources(
        libruqolacore
        PRIVATE
        activities/activitiesmanager.h
        activities/activitiesmanager.cpp
    )
    target_link_libraries(libruqolacore Plasma::Activities)
    if(BUILD_TESTING)
        add_subdirectory(activities/autotests)
    endif()
endif()

kconfig_add_kcfg_files(libruqolacore settings/ruqolaglobalconfig.kcfgc)

if(UNITY_SUPPORT)
    target_sources(
        libruqolacore
        PRIVATE
        unityservicemanager.cpp
        unityservicemanager.h
    )
    target_link_libraries(libruqolacore Qt::DBus)
endif()


if(USE_E2E_SUPPORT)
    target_link_libraries(libruqolacore OpenSSL::Crypto)
endif()

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_debug.h IDENTIFIER RUQOLA_LOG
    CATEGORY_NAME org.kde.ruqola DESCRIPTION "ruqola" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_message_debug.h IDENTIFIER RUQOLA_MESSAGE_LOG
    CATEGORY_NAME org.kde.ruqola.message DESCRIPTION "ruqola (show message debug)" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_ddpapi_debug.h IDENTIFIER RUQOLA_DDPAPI_LOG
    CATEGORY_NAME org.kde.ruqola.ddpapi DESCRIPTION "ruqola (ddpapi)" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_ddpapi_command_debug.h IDENTIFIER RUQOLA_DDPAPI_COMMAND_LOG
    CATEGORY_NAME org.kde.ruqola.ddpapi.command DESCRIPTION "ruqola (ddpapi command)" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_unknown_collectiontype_debug.h IDENTIFIER RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG
    CATEGORY_NAME org.kde.ruqola.ddp.collectiontype DESCRIPTION "ruqola ddp unknown collection" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_backend_debug.h IDENTIFIER RUQOLA_BACKEND_LOG
    CATEGORY_NAME org.kde.ruqola.ddp.backend DESCRIPTION "ruqola backend changed" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_typing_notification_debug.h IDENTIFIER RUQOLA_TYPING_NOTIFICATION_LOG
    CATEGORY_NAME org.kde.ruqola.typingnotification DESCRIPTION "ruqola typingnotification" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_completion_debug.h IDENTIFIER RUQOLA_COMPLETION_LOG
    CATEGORY_NAME org.kde.ruqola.completion DESCRIPTION "ruqola (completion)" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_notification_debug.h IDENTIFIER RUQOLA_NOTIFICATION_LOG
    CATEGORY_NAME org.kde.ruqola.notification DESCRIPTION "ruqola notification" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_commands_debug.h IDENTIFIER RUQOLA_COMMANDS_LOG
    CATEGORY_NAME org.kde.ruqola.commands DESCRIPTION "ruqola command" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_rooms_debug.h IDENTIFIER RUQOLA_ROOMS_LOG
    CATEGORY_NAME org.kde.ruqola.rooms DESCRIPTION "ruqola rooms" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_custom_sounds_debug.h IDENTIFIER RUQOLA_CUSTOMSOUNDS_LOG
    CATEGORY_NAME org.kde.ruqola.customsounds DESCRIPTION "ruqola custom sounds" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_away_debug.h IDENTIFIER RUQOLA_AWAY_LOG
    CATEGORY_NAME org.kde.ruqola.away DESCRIPTION "ruqola away manager" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_texttohtml_debug.h IDENTIFIER RUQOLA_TEXTTOHTML_LOG
    CATEGORY_NAME org.kde.ruqola.texttohtml DESCRIPTION "ruqola text to html support" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_texttohtml_cmark_debug.h IDENTIFIER RUQOLA_TEXTTOHTML_CMARK_LOG
    CATEGORY_NAME org.kde.ruqola.texttohtml.cmark DESCRIPTION "ruqola text to html support (cmark-rc lib)" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_specialwarning_debug.h IDENTIFIER RUQOLA_SPECIALWARNING_LOG
    CATEGORY_NAME org.kde.ruqola.specialwarning DESCRIPTION "ruqola special warning" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_thread_message_debug.h IDENTIFIER RUQOLA_THREAD_MESSAGE_LOG
    CATEGORY_NAME org.kde.ruqola.threadmessages DESCRIPTION "ruqola thread message" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_translation_debug.h IDENTIFIER RUQOLA_TRANSLATION_LOG
    CATEGORY_NAME org.kde.ruqola.translation DESCRIPTION "ruqola translation" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_password_core_debug.h IDENTIFIER RUQOLA_PASSWORD_CORE_LOG
    CATEGORY_NAME org.kde.ruqola.password DESCRIPTION "ruqola password support" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_reconnect_core_debug.h IDENTIFIER RUQOLA_RECONNECT_LOG
    CATEGORY_NAME org.kde.ruqola.reconnect DESCRIPTION "ruqola reconnect support" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_videoconference_core_debug.h IDENTIFIER RUQOLA_VIDEO_CONFERENCE_LOG
    CATEGORY_NAME org.kde.ruqola.videoconference DESCRIPTION "ruqola video conference support" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_database_debug.h IDENTIFIER RUQOLA_DATABASE_LOG
    CATEGORY_NAME org.kde.ruqola.database DESCRIPTION "ruqola database support" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_loadhistory_debug.h IDENTIFIER RUQOLA_LOAD_HISTORY_LOG
    CATEGORY_NAME org.kde.ruqola.loadhistory DESCRIPTION "ruqola load history" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_authentication_debug.h IDENTIFIER RUQOLA_AUTHENTICATION_LOG
    CATEGORY_NAME org.kde.ruqola.authentication DESCRIPTION "ruqola authentication" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_previewurlcache_debug.h IDENTIFIER RUQOLA_PREVIEWURLCACHE_LOG
    CATEGORY_NAME org.kde.ruqola.previewurlcache DESCRIPTION "ruqola preview url cache" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_encryption_debug.h IDENTIFIER RUQOLA_ENCRYPTION_LOG
    CATEGORY_NAME org.kde.ruqola.encryption DESCRIPTION "ruqola encryption" EXPORT RUQOLA
)
ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_restapi_authentication_debug.h IDENTIFIER RUQOLA_RESTAPI_AUTH_LOG
    CATEGORY_NAME org.kde.ruqola.restapi.authentication DESCRIPTION "ruqola (restapi authentication)" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_message_memory_debug.h IDENTIFIER RUQOLA_MESSAGE_MEMORY_LOG
    CATEGORY_NAME org.kde.ruqola.message.memory DESCRIPTION "ruqola message memory" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_room_memory_debug.h IDENTIFIER RUQOLA_ROOM_MEMORY_LOG
    CATEGORY_NAME org.kde.ruqola.room.memory DESCRIPTION "ruqola room memory" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_plasma_activities_debug.h IDENTIFIER RUQOLA_PLASMAACTIVITIES_LOG
    CATEGORY_NAME org.kde.ruqola.plasma.activities DESCRIPTION "ruqola plasma activities support" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_sound_debug.h IDENTIFIER RUQOLA_SOUND_LOG
    CATEGORY_NAME org.kde.ruqola.sound DESCRIPTION "ruqola sound support" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_message_url_debug.h IDENTIFIER RUQOLA_MESSAGE_URL_LOG
    CATEGORY_NAME org.kde.ruqola.messageurl DESCRIPTION "ruqola message url" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_memory_management_debug.h IDENTIFIER RUQOLA_MEMORY_MANAGEMENT_LOG
    CATEGORY_NAME org.kde.ruqola.memorymanagement DESCRIPTION "ruqola memory management" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_channel_management_debug.h IDENTIFIER RUQOLA_CHANNEL_MANAGEMENT_LOG
    CATEGORY_NAME org.kde.ruqola.channelmanagement DESCRIPTION "ruqola channel management" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_action_buttons_debug.h IDENTIFIER RUQOLA_ACTION_BUTTONS_LOG
    CATEGORY_NAME org.kde.ruqola.action_buttons DESCRIPTION "ruqola action buttons" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_messages_model_debug.h IDENTIFIER RUQOLA_MESSAGEMODELS_LOG
    CATEGORY_NAME org.kde.ruqola.messagemodels DESCRIPTION "ruqola message model" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_subscription_parsing_debug.h IDENTIFIER RUQOLA_SUBSCRIPTION_PARSING_LOG
    CATEGORY_NAME org.kde.ruqola.subscription_parsing DESCRIPTION "ruqola subscription parsing message" EXPORT RUQOLA
)

ecm_qt_declare_logging_category(libruqolacore_debug_SRCS HEADER ruqola_quick_search_debug.h IDENTIFIER RUQOLA_QUICK_SEARCH_LOG
    CATEGORY_NAME org.kde.ruqola.quick_search DESCRIPTION "ruqola quick search message" EXPORT RUQOLA
)

target_sources(libruqolacore PRIVATE ${libruqolacore_debug_SRCS})

if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(libruqolacore PROPERTIES UNITY_BUILD ON)
endif()

generate_export_header(libruqolacore BASE_NAME libruqolacore)

if(HAVE_NETWORKMANAGER)
    target_link_libraries(libruqolacore KF6::NetworkManagerQt)
endif()

target_link_libraries(
    libruqolacore
    Qt::Core
    Qt::Gui
    Qt::Sql
    Qt::WebSockets
    Qt::Network
    Qt::NetworkAuth
    KF6::CoreAddons
    KF6::I18n
    KF6::Notifications
    KF6::SyntaxHighlighting
    librocketchatrestapi-qt
    KF6::ConfigGui
    KF6::IdleTime
    KF6::WindowSystem
    qt6keychain
    KF6::StatusNotifierItem
    KF6::KIOCore
    Qt::Multimedia
    KF6::ColorScheme
)

if(NOT HAVE_TEXTUTILS_CMARK_SUPPORT)
    target_link_libraries(
        libruqolacore
        cmark-rc
    )
endif()

if(HAVE_TEXT_TRANSLATOR)
    target_link_libraries(libruqolacore KF6::TextTranslator)
endif()

if(HAVE_TEXT_AUTOCORRECTION_WIDGETS)
    target_link_libraries(libruqolacore KF6::TextAutoCorrectionWidgets)
endif()

if(HAVE_TEXT_UTILS)
    target_link_libraries(libruqolacore KF6::TextUtils)
endif()

if(WIN32 OR APPLE)
    target_link_libraries(libruqolacore KF6::IconThemes)
endif()
target_include_directories(libruqolacore PRIVATE ${QTKEYCHAIN_INCLUDE_DIRS})

if(TARGET KF6::TextEmoticonsCore)
    target_link_libraries(libruqolacore KF6::TextEmoticonsCore)
endif()

set_target_properties(
    libruqolacore
    PROPERTIES
        OUTPUT_NAME ruqolacore
        VERSION ${RUQOLA_LIB_VERSION}
        SOVERSION ${RUQOLA_LIB_SOVERSION}
)

if(BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(localdatabase/autotests)
    add_subdirectory(texttospeech/autotests)
endif()

if(USE_E2E_SUPPORT)
    add_subdirectory(encryption/tests)
endif()

install(
    TARGETS libruqolacore ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
    LIBRARY NAMELINK_SKIP
)
