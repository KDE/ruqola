# SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_library(librocketchatrestapi-qt5)
target_sources(librocketchatrestapi-qt5 PRIVATE
    connection.cpp
    restapiutil.cpp
    restapimethod.cpp
    uploadfilejob.cpp
    restapiabstractjob.cpp
    abstractlogger.cpp

    spotlightjob.cpp

    serverinfojob.cpp

    misc/owninfojob.cpp
    misc/settingsoauthjob.cpp
    misc/listoauthappsjob.cpp
    misc/listoauthappsjob.h
    misc/statisticsjob.cpp
    misc/directoryjob.cpp
    misc/roleslistjob.cpp
    misc/stdoutqueuejob.cpp
    misc/oauthappsjob.cpp
    misc/oauthappsjob.h

    users/userbasejob.cpp
    users/getavatarjob.cpp
    users/setavatarjob.cpp
    users/forgotpasswordjob.cpp
    users/userinfojob.cpp
    users/userspresencejob.cpp
    users/getpresencejob.cpp
    users/resetavatarjob.cpp
    users/getusernamesuggestionjob.cpp
    users/setstatusjob.cpp
    users/usersautocompletejob.cpp
    users/registeruserjob.cpp
    users/deleteownaccountjob.cpp
    users/usersupdatejob.cpp
    users/usersupdateownbasicinfojob.cpp
    users/userssetpreferencesjob.cpp
    users/removeothertokensjob.cpp
    users/deleteuserjob.cpp
    users/userscreatejob.cpp
    users/userslistjob.cpp
    users/setuseractivestatusjob.cpp

    authentication/logoutjob.cpp
    authentication/loginjob.cpp
    authentication/facebookauthjob.cpp
    authentication/googleauthjob.cpp
    authentication/twitterauthjob.cpp

    settings/privateinfojob.cpp
    settings/updateadminsettingsjob.cpp
    settings/updateadminsettingsjob.h

    channellistjob.cpp
    downloadfilejob.cpp

    emoji/loademojicustomjob.cpp
    emoji/emojicustomdeletejob.cpp
    emoji/emojicustomcreatejob.cpp
    emoji/emojicustomupdatejob.cpp
    emoji/emojicustomalljob.cpp

    chat/starmessagejob.cpp
    chat/syncthreadmessagesjob.cpp
    chat/pinmessagejob.cpp
    chat/postmessagejob.cpp
    chat/deletemessagejob.cpp
    chat/updatemessagejob.cpp
    chat/reactonmessagejob.cpp
    chat/searchmessagejob.cpp
    chat/ignoreuserjob.cpp
    chat/reportmessagejob.cpp
    chat/getmessagejob.cpp
    chat/followmessagejob.cpp
    chat/unfollowmessagejob.cpp
    chat/getthreadsjob.cpp
    chat/getthreadmessagesjob.cpp
    chat/sendmessagejob.cpp
    chat/getpinnedmessagesjob.cpp
    chat/getstarredmessagesjob.cpp
    chat/getsnippetedmessagesjob.cpp
    chat/getmentionedmessagesjob.cpp

    channels/changechanneltopicjob.cpp
    channels/changechannelannouncementjob.cpp
    channels/changechannelnamejob.cpp
    channels/createchanneljob.cpp
    channels/leavechanneljob.cpp
    channels/channelclosejob.cpp
    channels/channelhistoryjob.cpp
    channels/changechanneldescriptionjob.cpp
    channels/changechannelreadonlyjob.cpp
    channels/archivechanneljob.cpp
    channels/channelfilesjob.cpp
    channels/channelinvitejob.cpp
    channels/setjoincodechanneljob.cpp
    channels/channelremoveownerjob.cpp
    channels/setchanneltypejob.cpp
    channels/getchannelrolesjob.cpp
    channels/channeladdownerjob.cpp
    channels/channeladdmoderatorjob.cpp
    channels/channelkickjob.cpp
    channels/channeljoinjob.cpp
    channels/channelinfojob.cpp
    channels/channelgetallusermentionsjob.cpp
    channels/channelremovemoderatorjob.cpp
    channels/channeldeletejob.cpp
    channels/channelsmoderatorsjob.cpp
    channels/channelmembersjob.cpp
    channels/channelremoveleaderjob.cpp
    channels/channeladdleaderjob.cpp
    channels/channelgetcountersjob.cpp
    channels/channelopenjob.cpp

    groups/changegroupstopicjob.cpp
    groups/changegroupsannouncementjob.cpp
    groups/changegroupsnamejob.cpp
    groups/creategroupsjob.cpp
    groups/leavegroupsjob.cpp
    groups/changegroupsdescriptionjob.cpp
    groups/archivegroupsjob.cpp
    groups/groupsinvitejob.cpp
    groups/groupskickjob.cpp
    groups/setgrouptypejob.cpp
    groups/getgrouprolesjob.cpp
    groups/groupaddmoderatorjob.cpp
    groups/groupaddownerjob.cpp
    groups/groupsinfojob.cpp
    groups/groupremoveownerjob.cpp
    groups/groupaddleaderjob.cpp
    groups/groupremoveleaderjob.cpp
    groups/groupremovemoderatorjob.cpp
    groups/groupsdeletejob.cpp
    groups/changegroupsreadonlyjob.cpp
    groups/changegroupsencryptedjob.cpp
    groups/groupopenjob.cpp

    directmessage/createdmjob.cpp
    directmessage/opendmjob.cpp
    directmessage/settopicdmjob.cpp
    directmessage/deletedmjob.cpp
    directmessage/deletedmjob.h

    rooms/savenotificationjob.cpp
    rooms/getroomsjob.cpp
    rooms/roomfavoritejob.cpp
    rooms/roomsinfojob.cpp
    rooms/roomleavejob.cpp
    rooms/roomstartdiscussionjob.cpp
    rooms/getdiscussionsjob.cpp
    rooms/roomsautocompletechannelandprivatejob.cpp
    rooms/adminroomsjob.cpp
    rooms/roomscleanhistoryjob.cpp
    rooms/roomsexportjob.cpp
    rooms/saveroomsettingsjob.cpp
    rooms/changearchivationstatejob.cpp
    rooms/adminroomsgetroomjob.cpp
    rooms/adminroomsgetroomjob.h

    subscriptions/markroomasreadjob.cpp
    subscriptions/markroomasunreadjob.cpp

    permissions/permissionslistalljob.cpp
    permissions/permissionupdatejob.cpp

    commands/listcommandsjob.cpp
    commands/getcommandsjob.cpp
    commands/runcommandjob.cpp

    e2e/fetchmykeysjob.cpp
    e2e/setuserpublicandprivatekeysjob.cpp

    video-conference/videoconfupdatejitsitimeoutjob.cpp

    autotranslate/getsupportedlanguagesjob.cpp
    autotranslate/translatemessagejob.cpp
    autotranslate/translatesavesettingsjob.cpp

    custom/customuserstatuslistjob.cpp
    custom/customuserstatuscreatejob.cpp
    custom/customuserstatusdeletejob.cpp
    custom/customuserstatusupdatejob.cpp

    custom/customsoundslistjob.cpp

    invite/findorcreateinvitejob.cpp
    invite/listinvitejob.cpp
    invite/removeinvitejob.cpp

    2fa/user2faenableemailjob.cpp
    2fa/user2fadisableemailjob.cpp
    2fa/user2fasendemailcodejob.cpp

    teams/teamslistroomsjob.cpp
    teams/teamremoveroomjob.cpp
    teams/teamupdateroomjob.cpp
    teams/teamleaveroomjob.cpp
    teams/roomsautocompleteavailableforteamsjob.cpp
    teams/teamaddroomsjob.cpp
    teams/teamscreatejob.cpp
    teams/teamremovemembersjob.cpp
    teams/channelsconverttoteamjob.cpp
    teams/teamdeletejob.cpp
    teams/teaminfojob.cpp
    teams/groupsconverttoteamjob.cpp
    teams/teamsautocompletejob.cpp
    teams/teamslistjob.cpp
    teams/teamconverttochanneljob.cpp

    createchannelteaminfo.cpp
    createupdateuserinfo.cpp
    channelgroupbasejob.cpp

    license/licenseslistjob.cpp

    role/rolecreatejob.cpp
    role/roleupdatejob.cpp
    role/roledeletejob.cpp
    role/removeuserfromrolejob.cpp 
    role/addusertorolejob.cpp
    role/getusersinrolejob.cpp

    misc/settingsoauthjob.h
    misc/statisticsjob.h
    misc/directoryjob.h
    misc/stdoutqueuejob.h
    misc/owninfojob.h
    misc/roleslistjob.h
    channels/channeljoinjob.h
    channels/channelremoveownerjob.h
    channels/channelinfojob.h
    channels/changechannelannouncementjob.h
    channels/setchanneltypejob.h
    channels/channelmembersjob.h
    channels/channelremoveleaderjob.h
    channels/channelremovemoderatorjob.h
    channels/getchannelrolesjob.h
    channels/channelgetallusermentionsjob.h
    channels/channelsmoderatorsjob.h
    channels/changechannelreadonlyjob.h
    channels/channelclosejob.h
    channels/leavechanneljob.h
    channels/channeladdleaderjob.h
    channels/changechanneldescriptionjob.h
    channels/channeldeletejob.h
    channels/setjoincodechanneljob.h
    channels/createchanneljob.h
    channels/channeladdownerjob.h
    channels/archivechanneljob.h
    channels/channeladdmoderatorjob.h
    channels/channelgetcountersjob.h
    channels/channelhistoryjob.h
    channels/channelinvitejob.h
    channels/changechanneltopicjob.h
    channels/channelfilesjob.h
    channels/changechannelnamejob.h
    channels/channelopenjob.h
    channels/channelkickjob.h
    restapiabstractjob.h
    license/licenseslistjob.h
    serverinfojob.h
    subscriptions/markroomasunreadjob.h
    subscriptions/markroomasreadjob.h
    authentication/facebookauthjob.h
    authentication/loginjob.h
    authentication/twitterauthjob.h
    authentication/googleauthjob.h
    authentication/logoutjob.h
    commands/runcommandjob.h
    commands/listcommandsjob.h
    commands/getcommandsjob.h
    users/userspresencejob.h
    users/getusernamesuggestionjob.h
    users/userslistjob.h
    users/resetavatarjob.h
    users/userssetpreferencesjob.h
    users/setstatusjob.h
    users/getpresencejob.h
    users/usersupdatejob.h
    users/userscreatejob.h
    users/setavatarjob.h
    users/removeothertokensjob.h
    users/userbasejob.h
    users/forgotpasswordjob.h
    users/getavatarjob.h
    users/deleteownaccountjob.h
    users/setuseractivestatusjob.h
    users/deleteuserjob.h
    users/userinfojob.h
    users/registeruserjob.h
    users/usersupdateownbasicinfojob.h
    users/usersautocompletejob.h
    directmessage/createdmjob.h
    directmessage/settopicdmjob.h
    directmessage/opendmjob.h
    emoji/emojicustomdeletejob.h
    emoji/emojicustomalljob.h
    emoji/emojicustomcreatejob.h
    emoji/emojicustomupdatejob.h
    emoji/loademojicustomjob.h
    role/rolecreatejob.h
    role/removeuserfromrolejob.h
    role/roleupdatejob.h
    role/roledeletejob.h
    role/addusertorolejob.h
    role/getusersinrolejob.h
    channelgroupbasejob.h
    chat/pinmessagejob.h
    chat/getthreadsjob.h
    chat/postmessagejob.h
    chat/getmessagejob.h
    chat/getstarredmessagesjob.h
    chat/updatemessagejob.h
    chat/sendmessagejob.h
    chat/syncthreadmessagesjob.h
    chat/reportmessagejob.h
    chat/reactonmessagejob.h
    chat/ignoreuserjob.h
    chat/getthreadmessagesjob.h
    chat/getmentionedmessagesjob.h
    chat/deletemessagejob.h
    chat/followmessagejob.h
    chat/starmessagejob.h
    chat/unfollowmessagejob.h
    chat/getpinnedmessagesjob.h
    chat/searchmessagejob.h
    chat/getsnippetedmessagesjob.h
    rooms/roomsinfojob.h
    rooms/getdiscussionsjob.h
    rooms/roomleavejob.h
    rooms/roomstartdiscussionjob.h
    rooms/roomscleanhistoryjob.h
    rooms/getroomsjob.h
    rooms/saveroomsettingsjob.h
    rooms/savenotificationjob.h
    rooms/roomfavoritejob.h
    rooms/changearchivationstatejob.h
    rooms/roomsexportjob.h
    rooms/roomsautocompletechannelandprivatejob.h
    rooms/adminroomsjob.h
    autotranslate/translatesavesettingsjob.h
    autotranslate/getsupportedlanguagesjob.h
    autotranslate/translatemessagejob.h
    restapimethod.h
    createupdateuserinfo.h
    custom/customuserstatusdeletejob.h
    custom/customuserstatuslistjob.h
    custom/customuserstatuscreatejob.h
    custom/customuserstatusupdatejob.h
    custom/customsoundslistjob.h
    permissions/permissionupdatejob.h
    permissions/permissionslistalljob.h
    python/pyrocketchatrestapi-qt5/rocketchatrestapi-qt5_global.h
    groups/getgrouprolesjob.h
    groups/groupaddmoderatorjob.h
    groups/groupsdeletejob.h
    groups/groupsinvitejob.h
    groups/leavegroupsjob.h
    groups/groupaddownerjob.h
    groups/creategroupsjob.h
    groups/archivegroupsjob.h
    groups/changegroupsannouncementjob.h
    groups/groupskickjob.h
    groups/groupremoveownerjob.h
    groups/groupremovemoderatorjob.h
    groups/groupsinfojob.h
    groups/changegroupsencryptedjob.h
    groups/changegroupsnamejob.h
    groups/changegroupstopicjob.h
    groups/groupopenjob.h
    groups/changegroupsreadonlyjob.h
    groups/groupaddleaderjob.h
    groups/setgrouptypejob.h
    groups/groupremoveleaderjob.h
    groups/changegroupsdescriptionjob.h
    downloadfilejob.h
    spotlightjob.h
    channellistjob.h
    e2e/fetchmykeysjob.h
    e2e/setuserpublicandprivatekeysjob.h
    createchannelteaminfo.h
    abstractlogger.h
    invite/removeinvitejob.h
    invite/findorcreateinvitejob.h
    invite/listinvitejob.h
    video-conference/videoconfupdatejitsitimeoutjob.h
    uploadfilejob.h
    librestapi_private_export.h
    2fa/user2fadisableemailjob.h
    2fa/user2fasendemailcodejob.h
    2fa/user2faenableemailjob.h
    connection.h
    teams/roomsautocompleteavailableforteamsjob.h
    teams/teamleaveroomjob.h
    teams/teamremoveroomjob.h
    teams/channelsconverttoteamjob.h
    teams/teamremovemembersjob.h
    teams/teamslistjob.h
    teams/teamconverttochanneljob.h
    teams/teamdeletejob.h
    teams/teamaddroomsjob.h
    teams/teamscreatejob.h
    teams/teaminfojob.h
    teams/teamsautocompletejob.h
    teams/groupsconverttoteamjob.h
    teams/teamslistroomsjob.h
    teams/teamupdateroomjob.h
    settings/privateinfojob.h
    restapiutil.h


    e2e/resetowne2ekeyjob.cpp
    e2e/resetowne2ekeyjob.h

    users/resettotpjob.cpp
    users/resettotpjob.h

    users/resete2ekeyjob.cpp
    users/resete2ekeyjob.h
    )


ecm_qt_declare_logging_category(librocketchatrestapi_qt5_debug_SRCS HEADER rocketchatqtrestapi_debug.h IDENTIFIER ROCKETCHATQTRESTAPI_LOG CATEGORY_NAME org.kde.rocketchatqtrestapi
    DESCRIPTION "RocketChatQtRestApi (Rocket Chat Qt Rest API used by ruqola)" EXPORT RUQOLA)
target_sources(librocketchatrestapi-qt5 PRIVATE ${librocketchatrestapi_qt5_debug_SRCS})

#ruqola_target_precompile_headers(librocketchatrestapi-qt5 PUBLIC ../../ruqola_pch.h)
generate_export_header(librocketchatrestapi-qt5 BASE_NAME librocketchatrestapi-qt5)
target_link_libraries(librocketchatrestapi-qt5
    Qt5::Core
    Qt5::Network
    Qt5::NetworkAuth
    KF5::I18n
)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(librocketchatrestapi-qt5 PROPERTIES UNITY_BUILD ON)
endif()
set_target_properties(librocketchatrestapi-qt5
    PROPERTIES OUTPUT_NAME rocketchatrestapi-qt5 VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
    )

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

install(TARGETS librocketchatrestapi-qt5 ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

if(OPTION_BUILD_PYTHON_BINDINGS)
    add_subdirectory(python)
endif()


