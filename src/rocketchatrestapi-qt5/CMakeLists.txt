# SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_library(librocketchatrestapi-qt5)
target_sources(librocketchatrestapi-qt5 PRIVATE
    restapirequest.cpp
    restapiutil.cpp
    restapimethod.cpp
    uploadfilejob.cpp
    restapiabstractjob.cpp
    abstractlogger.cpp

    spotlightjob.cpp

    serverinfojob.cpp

    misc/owninfojob.cpp
    misc/settingsoauthjob.cpp
    misc/statisticsjob.cpp
    misc/directoryjob.cpp
    misc/roleslistjob.cpp
    misc/stdoutqueuejob.cpp

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

    channellistjob.cpp
    downloadfilejob.cpp

    emoji/loademojicustomjob.cpp
    emoji/emojicustomdeletejob.cpp
    emoji/emojicustomcreatejob.cpp
    emoji/emojicustomupdatejob.cpp

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
    channels/channelbasejob.cpp
    channels/channelgetcountersjob.cpp

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

    directmessage/createdmjob.cpp
    directmessage/opendmjob.cpp
    directmessage/settopicdmjob.cpp

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

    subscriptions/markroomasreadjob.cpp
    subscriptions/markroomasunreadjob.cpp

    permissions/permissionslistalljob.cpp

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

    createroominfo.cpp
    createupdateuserinfo.cpp
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


