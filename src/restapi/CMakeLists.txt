
set (Restapi_SRCS
    restapirequest.cpp
    restapiutil.cpp
    restapimethod.cpp
    uploadfilejob.cpp
    restapiabstractjob.cpp

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

    directmessage/createdmjob.cpp

    rooms/savenotificationjob.cpp
    rooms/getroomsjob.cpp
    rooms/roomfavoritejob.cpp

    subscriptions/markroomasreadjob.cpp
    subscriptions/markroomasunreadjob.cpp

    permissions/listpermissionsjob.cpp

    commands/listcommandsjob.cpp

    abstractlogger.cpp

    )


ecm_qt_declare_logging_category(Restapi_SRCS HEADER restapi_debug.h IDENTIFIER RESTAPI_LOG CATEGORY_NAME org.kde.restapi)
add_library(librestapi ${Restapi_SRCS})
generate_export_header(librestapi BASE_NAME librestapi)
target_link_libraries(librestapi
    Qt5::Core
    Qt5::WebSockets
    Qt5::Network
    Qt5::NetworkAuth
)

set_target_properties(librestapi
    PROPERTIES OUTPUT_NAME restapi VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
    )


install(TARGETS librestapi ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

