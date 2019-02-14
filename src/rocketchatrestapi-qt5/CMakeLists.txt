
set (RocketChatRestApiQt_SRCS
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

    users/getavatarjob.cpp
    users/setavatarjob.cpp
    users/forgotpasswordjob.cpp
    users/usersinfojob.cpp
    users/getpresencejob.cpp
    users/resetavatarjob.cpp
    users/getusernamesuggestionjob.cpp

    authentication/logoutjob.cpp
    authentication/loginjob.cpp
    authentication/facebookauthjob.cpp
    authentication/googleauthjob.cpp
    authentication/twitterauthjob.cpp

    settings/privateinfojob.cpp

    channellistjob.cpp
    downloadfilejob.cpp

    emoji/loademojicustomjob.cpp

    chat/starmessagejob.cpp
    chat/pinmessagejob.cpp
    chat/postmessagejob.cpp
    chat/deletemessagejob.cpp
    chat/updatemessagejob.cpp
    chat/reactonmessagejob.cpp
    chat/searchmessagejob.cpp
    chat/ignoreuserjob.cpp
    chat/reportmessagejob.cpp
    chat/getmessagejob.cpp

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
    channels/changechannelencryptedjob.cpp

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

    subscriptions/markroomasreadjob.cpp
    subscriptions/markroomasunreadjob.cpp

    permissions/listpermissionsjob.cpp

    commands/listcommandsjob.cpp

    e2e/fetchmykeysjob.cpp
    e2e/setuserpublicandprivatekeysjob.cpp

    video-conference/videoconfupdatejitsitimeoutjob.cpp
    )


ecm_qt_declare_logging_category(RocketChatRestApiQt_SRCS HEADER rocketchatqtrestapi_debug.h IDENTIFIER ROCKETCHATQTRESTAPI_LOG CATEGORY_NAME org.kde.rocketchatqtrestapi)
add_library(librocketchatrestapi-qt5 ${RocketChatRestApiQt_SRCS})
generate_export_header(librocketchatrestapi-qt5 BASE_NAME librocketchatrestapi-qt5)
target_link_libraries(librocketchatrestapi-qt5
    Qt5::Core
    Qt5::Network
    Qt5::NetworkAuth
    KF5::I18n
)

set_target_properties(librocketchatrestapi-qt5
    PROPERTIES OUTPUT_NAME rocketchatrestapi-qt5 VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
    )

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

install(TARGETS librocketchatrestapi-qt5 ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

