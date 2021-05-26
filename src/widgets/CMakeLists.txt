# SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_library(libruqolawidgets)

if (TARGET KUserFeedbackWidgets)
    target_sources(libruqolawidgets PRIVATE
        userfeedback/userfeedbackmanager.cpp
        userfeedback/ruqolauserfeedbackprovider.cpp
        userfeedback/accountinfosource.cpp
        )
endif()


target_sources(libruqolawidgets PRIVATE
    ruqolamainwindow.cpp
    ruqolacentralwidget.cpp
    ruqolaloginwidget.cpp
    ruqolamainwidget.cpp

    registeruser/registeruserdialog.cpp
    registeruser/registeruserwidget.cpp

    dialogs/serverinfo/serverinfowidget.cpp
    dialogs/serverinfo/serverinfodialog.cpp
    dialogs/uploadfiledialog.cpp
    dialogs/uploadfilewidget.cpp
    dialogs/channelinfowidget.cpp
    dialogs/channelinfodialog.cpp
    dialogs/searchchanneldialog.cpp
    dialogs/searchchannelwidget.cpp
    dialogs/modifystatusdialog.cpp
    dialogs/modifystatuswidget.cpp
    dialogs/createnewchanneldialog.cpp
    dialogs/createnewchannelwidget.cpp
    dialogs/showlistmessagebasedialog.cpp
    dialogs/showlistmessagebasewidget.cpp
    dialogs/createnewserverdialog.cpp
    dialogs/createnewserverwidget.cpp
    dialogs/showpinnedmessagesdialog.cpp
    dialogs/showstarredmessagesdialog.cpp
    dialogs/showmentionsmessagesdialog.cpp
    dialogs/showsnipperedmessagesdialog.cpp
    dialogs/configurenotificationdialog.cpp
    dialogs/configurenotificationwidget.cpp
    dialogs/searchmessagedialog.cpp
    dialogs/searchmessagewidget.cpp
    dialogs/reportmessagedialog.cpp
    dialogs/reportmessagewidget.cpp
    dialogs/showimagedialog.cpp
    dialogs/showimagewidget.cpp
    dialogs/showattachmentdialog.cpp
    dialogs/showattachmentwidget.cpp
    dialogs/directchannelinfodialog.cpp
    dialogs/directchannelinfowidget.cpp
    dialogs/attachment/listattachmentdelegate.cpp
    dialogs/showdiscussionsdialog.cpp
    dialogs/showdiscussionswidget.cpp
    dialogs/discussion/listdiscussiondelegate.cpp
    dialogs/createnewdiscussiondialog.cpp
    dialogs/createnewdiscussionwidget.cpp
    dialogs/channelpassworddialog.cpp
    dialogs/channelpasswordwidget.cpp
    dialogs/addusersinroomdialog.cpp
    dialogs/addusersinroomwidget.cpp
    dialogs/adduserscompletionlineedit.cpp
    dialogs/showthreadsdialog.cpp
    dialogs/searchchannel/searchchanneldelegate.cpp
    dialogs/inviteusersdialog.cpp
    dialogs/inviteuserswidget.cpp
    dialogs/autotranslateconfiguredialog.cpp
    dialogs/autotranslateconfigurewidget.cpp
    dialogs/showvideowidget.cpp
    dialogs/showvideodialog.cpp
    dialogs/playsounddialog.cpp
    dialogs/playsoundwidget.cpp
    dialogs/createvideomessagedialog.cpp
    dialogs/createvideomessagewidget.cpp
    dialogs/showattachmentcombobox.cpp
    dialogs/asktwoauthenticationpassworddialog.cpp
    dialogs/asktwoauthenticationpasswordwidget.cpp
    dialogs/createdirectmessagesdialog.cpp
    dialogs/createdirectmessageswidget.cpp
    dialogs/channelinfoprunewidget.cpp
    dialogs/searchchannellistview.cpp
    dialogs/channelnamevalidlineedit.cpp
    dialogs/channelsearchnamelineedit.cpp
    dialogs/channelsearchwidget.cpp
    dialogs/channelsearchnamelineresultwidget.cpp
    dialogs/channelinforeadonlywidget.cpp
    dialogs/channelinfoeditablewidget.cpp
    dialogs/roomavatarwidget.cpp
    dialogs/messagetexteditor.cpp

    configuredialog/configuresettingsdialog.cpp
    configuredialog/configureaccountwidget.cpp
    configuredialog/accountserverlistwidget.cpp
    configuredialog/configureaccountserverwidget.cpp
    configuredialog/configurespellcheckingwidget.cpp
    configuredialog/configureuserfeedbackwidget.cpp
    configuredialog/configuregeneralwidget.cpp
    configuredialog/configurefontwidget.cpp

    threadwidget/threadmessagewidget.cpp
    threadwidget/threadmessagedialog.cpp

    channellist/channellistwidget.cpp
    channellist/channellistview.cpp
    channellist/channellistdelegate.cpp

    prunemessages/prunemessagesdialog.cpp
    prunemessages/prunemessageswidget.cpp

    room/roomwidget.cpp
    room/roomheaderwidget.cpp
    room/messagelistview.cpp
    room/messagetextedit.cpp
    room/delegate/messagelistdelegate.cpp
    room/delegate/messagedelegatehelperbase.cpp
    room/delegate/messagedelegatehelpertext.cpp
    room/delegate/messageattachmentdelegatehelpertext.cpp
    room/delegate/messageattachmentdelegatehelperimage.cpp
    room/delegate/messageattachmentdelegatehelperfile.cpp
    room/delegate/messagedelegatehelperreactions.cpp
    room/delegate/messageattachmentdelegatehelpervideo.cpp
    room/delegate/messageattachmentdelegatehelpersound.cpp
    room/delegate/runninganimatedimage.cpp
    room/delegate/messagedelegateutils.cpp
    room/messagelinewidget.cpp
    room/readonlylineeditwidget.cpp
    room/usersinroomflowwidget.cpp
    room/usersinroomlabel.cpp
    room/channelactionpopupmenu.cpp
    room/roomutil.cpp
    room/roomcounterinfowidget.cpp
    room/roomreplythreadwidget.cpp
    room/roomquotemessagewidget.cpp
    room/usersinroomdialog.cpp
    room/usersinroomwidget.cpp
    room/usersinroommenu.cpp
    room/usersinroomcombobox.cpp
    room/uploadfileprogressstatuswidget.cpp
    room/roomwidgetbase.cpp

    misc/pixmapcache.cpp
    misc/avatarcachemanager.cpp
    misc/servermenu.cpp
    misc/accountsoverviewwidget.cpp
    misc/emoticonmenuwidget.cpp
    misc/emoticonselectorwidget.cpp
    misc/emoticonrecentusedfilterproxymodel.cpp
    misc/adduserswidget.cpp
    misc/clickablewidget.cpp
    misc/recentusedemoticonview.cpp
    misc/lineeditcatchreturnkey.cpp
    misc/passwordlineeditwidget.cpp
    misc/passwordconfirmwidget.cpp
    misc/twoauthenticationpasswordwidget.cpp
    misc/statuscombobox.cpp
    misc/systemmessagescombobox.cpp
    misc/searchwithdelaylineedit.cpp

    common/completionlineedit.cpp
    common/completionlistview.cpp
    common/emojicompletiondelegate.cpp
    common/authenticationcombobox.cpp
    common/flowlayout.cpp
    common/commandcompletiondelegate.cpp
    common/delegatepaintutil.cpp
    common/delegateutil.cpp

    room/textpluginmanager.cpp
    room/plugins/plugintext.cpp
    room/plugins/plugintextinterface.cpp

    myaccount/myaccountconfiguredialog.cpp
    myaccount/myaccountconfigurewidget.cpp
    myaccount/myaccount2faconfigurewidget.cpp
    myaccount/myaccountprofileconfigurewidget.cpp
    myaccount/myaccountpreferenceconfigurewidget.cpp
    myaccount/myaccountprofileconfigureavatarwidget.cpp

    administratordialog/administratordialog.cpp
    administratordialog/administratorwidget.cpp
    administratordialog/rooms/administratorroomswidget.cpp
    administratordialog/serverinfo/administratorserverinfowidget.cpp
    administratordialog/rooms/administratorroomsselectroomtypewidget.cpp
    administratordialog/customuserstatus/administratorcustomuserstatuswidget.cpp
    administratordialog/customuserstatus/administratorcustomuserstatuscreatedialog.cpp
    administratordialog/customuserstatus/administratorcustomuserstatuscreatewidget.cpp
    administratordialog/customuserstatus/customuserstatustreewidget.cpp

    administratordialog/customsounds/administratorcustomsoundswidget.cpp
    administratordialog/customsounds/administratorcustomsoundscreatedialog.cpp
    administratordialog/customsounds/administratorcustomsoundscreatewidget.cpp
    administratordialog/customsounds/customsoundstreewidget.cpp

    administratordialog/users/administratoruserswidget.cpp

    administratordialog/invites/administratorinviteswidget.cpp
    administratordialog/invites/invitetreeview.cpp

    exportmessages/exportmessagesdialog.cpp
    exportmessages/exportmessageswidget.cpp

    teams/teamchannelsdialog.cpp
    teams/teamchannelswidget.cpp
    teams/teamchannelscombobox.cpp
    teams/teamsearchroomdialog.cpp
    teams/teamsearchroomwidget.cpp
    teams/teamsearchroomforteamwidget.cpp
    teams/addteamroomcompletionlineedit.cpp
    teams/teamselectdeletedroomdialog.cpp
    teams/teamselectdeletedroomwidget.cpp
    teams/searchteamdialog.cpp
    teams/searchteamwidget.cpp
    teams/searchteamcompletionlineedit.cpp

    directory/directorydialog.cpp
    directory/directorywidget.cpp
    directory/directorytabwidget.cpp

    ruqolawidget.qrc
    )


ki18n_wrap_ui(libruqolawidgets
    configuredialog/configureaccountserverwidget.ui
    )



ecm_qt_declare_logging_category(libruqolawidgets HEADER ruqolawidgets_debug.h IDENTIFIER RUQOLAWIDGETS_LOG CATEGORY_NAME org.kde.ruqola.widgets
    DESCRIPTION "ruqola widgets" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolawidgets HEADER ruqolawidgets_selection_debug.h IDENTIFIER RUQOLAWIDGETS_SELECTION_LOG CATEGORY_NAME org.kde.ruqola.widgets.selection
    DESCRIPTION "ruqola widgets (selection)" EXPORT RUQOLA)

ruqola_target_precompile_headers(libruqolawidgets PUBLIC ../../ruqola_pch.h)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(libruqolawidgets PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(libruqolawidgets BASE_NAME libruqolawidgets)

if (TARGET KUserFeedbackWidgets)
        target_link_libraries(libruqolawidgets KUserFeedbackWidgets)
endif()


target_link_libraries(libruqolawidgets
    Qt5::Gui
    Qt5::Widgets
    Qt5::MultimediaWidgets
    KF5::I18n
    KF5::ConfigCore
    KF5::XmlGui
    KF5::KIOWidgets
    KF5::WidgetsAddons
    KF5::SonnetUi
    KF5::TextWidgets
    KF5::NotifyConfig
    KF5::ItemViews
    librocketchatrestapi-qt5
    libruqolacore
)

if (NOT WIN32)
    target_link_libraries(libruqolawidgets Qt5::DBus)
endif()


set_target_properties(libruqolawidgets
    PROPERTIES OUTPUT_NAME ruqolawidgets VERSION ${RUQOLA_LIB_VERSION} SOVERSION ${RUQOLA_LIB_SOVERSION}
    )

if (BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
    add_subdirectory(channellist/autotests)
    add_subdirectory(room/autotests)
    add_subdirectory(dialogs/autotests)
    add_subdirectory(misc/autotests)
    add_subdirectory(configuredialog/autotests)
    add_subdirectory(common/autotests)
    add_subdirectory(threadwidget/autotests)
    add_subdirectory(registeruser/autotests)
    add_subdirectory(myaccount/autotests)
    add_subdirectory(prunemessages/autotests)
    add_subdirectory(administratordialog/autotests)
    add_subdirectory(exportmessages/autotests)
    add_subdirectory(teams/autotests)
    add_subdirectory(directory/autotests)
endif()

install(TARGETS libruqolawidgets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
