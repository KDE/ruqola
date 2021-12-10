# SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_library(libruqolawidgets)

if (TARGET KUserFeedbackWidgets)
    target_sources(libruqolawidgets PRIVATE
        userfeedback/userfeedbackmanager.cpp
        userfeedback/ruqolauserfeedbackprovider.cpp
        userfeedback/accountinfosource.cpp
        userfeedback/ruqolauserfeedbackprovider.h
        userfeedback/accountinfosource.h
        userfeedback/userfeedbackmanager.h

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
    room/delegate/textselection.cpp
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
    room/teamnamelabel.cpp
    room/roomheaderlabel.cpp
    room/reconnectinfowidget.cpp

    misc/pixmapcache.cpp
    misc/avatarcachemanager.cpp
    misc/servermenu.cpp
    misc/accountsoverviewwidget.cpp
    misc/emoticonlistview.cpp
    misc/emoticonmenuwidget.cpp
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
    misc/searchtreebasewidget.cpp
    misc/rolescombobox.cpp

    common/completionlineedit.cpp
    common/completionlistview.cpp
    common/emojicompletiondelegate.cpp
    common/authenticationcombobox.cpp
    common/flowlayout.cpp
    common/commandcompletiondelegate.cpp
    common/delegatepaintutil.cpp
    common/delegateutil.cpp
    common/emojicustomdelegate.cpp

    room/textpluginmanager.cpp
    room/plugins/plugintext.cpp
    room/plugins/plugintextinterface.cpp

    myaccount/myaccountconfiguredialog.cpp
    myaccount/myaccountconfigurewidget.cpp
    myaccount/myaccount2faconfigurewidget.cpp
    myaccount/myaccountprofileconfigurewidget.cpp
    myaccount/myaccountpreferenceconfigurewidget.cpp
    myaccount/myaccountprofileconfigureavatarwidget.cpp
    myaccount/myaccountsecurityconfigurewidget.cpp
    myaccount/myaccount2e2configurewidget.h
    myaccount/myaccount2e2configurewidget.cpp

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

    administratordialog/users/administratoruserswidget.cpp
    administratordialog/users/administratoradduserdialog.cpp
    administratordialog/users/administratoradduserwidget.cpp

    administratordialog/invites/administratorinviteswidget.cpp
    administratordialog/invites/administratorinvitesfilterproxymodel.cpp
    administratordialog/invites/invitetreeview.cpp

    administratordialog/logs/viewlogwidget.cpp

    administratordialog/permissions/permissionswidget.cpp
    administratordialog/permissions/permissionseditdialog.cpp
    administratordialog/permissions/permissionseditwidget.cpp
    administratordialog/permissions/permissionstreeview.cpp

    administratordialog/customemoji/administratorcustomemojiwidget.cpp
    administratordialog/customemoji/administratorcustomemojicreatedialog.cpp
    administratordialog/customemoji/administratorcustomemojicreatewidget.cpp

    administratordialog/roles/administratorroleswidget.cpp
    administratordialog/roles/rolestreeview.cpp
    administratordialog/roles/roleeditdialog.cpp
    administratordialog/roles/roleeditwidget.cpp
    administratordialog/roles/rolescopecombobox.cpp
    administratordialog/roles/userinroleeditdialog.cpp
    administratordialog/roles/usersinrolewidget.cpp

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
    teams/teamconverttochannelwidget.cpp
    teams/teamconverttochanneldialog.cpp

    directory/directorydialog.cpp
    directory/directorywidget.cpp
    directory/directorytabwidget.cpp

    otr/otrwidget.cpp

    ruqolawidget.qrc
    exportmessages/exportmessagesdialog.h
    exportmessages/exportmessageswidget.h
    misc/clickablewidget.h
    misc/emoticonmenuwidget.h
    misc/emoticonrecentusedfilterproxymodel.h
    misc/lineeditcatchreturnkey.h
    misc/statuscombobox.h
    misc/servermenu.h
    misc/emoticonlistview.h
    misc/passwordlineeditwidget.h
    misc/rolescombobox.h
    misc/recentusedemoticonview.h
    misc/pixmapcache.h
    misc/accountsoverviewwidget.h
    misc/twoauthenticationpasswordwidget.h
    misc/passwordconfirmwidget.h
    misc/adduserswidget.h
    misc/systemmessagescombobox.h
    misc/searchwithdelaylineedit.h
    misc/searchtreebasewidget.h
    misc/avatarcachemanager.h
    channellist/channellistwidget.h
    channellist/channellistdelegate.h
    channellist/channellistview.h
    ruqolamainwidget.h
    configuredialog/configurefontwidget.h
    configuredialog/configureaccountserverwidget.h
    configuredialog/accountserverlistwidget.h
    configuredialog/configureuserfeedbackwidget.h
    configuredialog/configurespellcheckingwidget.h
    configuredialog/configuresettingsdialog.h
    configuredialog/configuregeneralwidget.h
    configuredialog/configureaccountwidget.h
    common/commandcompletiondelegate.h
    common/completionlistview.h
    common/authenticationcombobox.h
    common/emojicompletiondelegate.h
    common/delegatepaintutil.h
    common/emojicustomdelegate.h
    common/flowlayout.h
    common/delegateutil.h
    common/completionlineedit.h
    room/delegate/messageattachmentdelegatehelperimage.h
    room/delegate/messagedelegatehelpertext.h
    room/delegate/messageattachmentdelegatehelperfile.h
    room/delegate/messagedelegatehelperreactions.h
    room/delegate/messageattachmentdelegatehelpersound.h
    room/delegate/messageattachmentdelegatehelpervideo.h
    room/delegate/messageattachmentdelegatehelpertext.h
    room/delegate/runninganimatedimage.h
    room/delegate/messagedelegatehelperbase.h
    room/delegate/messagedelegateutils.h
    room/delegate/messagelistdelegate.h
    room/delegate/textselection.h
    room/usersinroomflowwidget.h
    room/roomreplythreadwidget.h
    room/teamnamelabel.h
    room/roomutil.h
    room/reconnectinfowidget.h
    room/usersinroomcombobox.h
    room/usersinroommenu.h
    room/uploadfileprogressstatuswidget.h
    room/usersinroomwidget.h
    room/usersinroomdialog.h
    room/roomwidget.h
    room/roomcounterinfowidget.h
    room/roomwidgetbase.h
    room/roomquotemessagewidget.h
    room/roomheaderwidget.h
    room/messagelinewidget.h
    room/readonlylineeditwidget.h
    room/roomheaderlabel.h
    room/plugins/plugintext.h
    room/plugins/plugintextinterface.h
    room/textpluginmanager.h
    room/usersinroomlabel.h
    room/messagetextedit.h
    room/channelactionpopupmenu.h
    room/messagelistview.h
    otr/otrwidget.h
    registeruser/registeruserdialog.h
    registeruser/registeruserwidget.h
    threadwidget/threadmessagedialog.h
    threadwidget/threadmessagewidget.h
    ruqolamainwindow.h
    ruqolaloginwidget.h
    myaccount/myaccountprofileconfigurewidget.h
    myaccount/myaccountconfigurewidget.h
    myaccount/myaccountsecurityconfigurewidget.h
    myaccount/myaccountprofileconfigureavatarwidget.h
    myaccount/myaccount2faconfigurewidget.h
    myaccount/myaccountpreferenceconfigurewidget.h
    myaccount/myaccountconfiguredialog.h
    administratordialog/users/administratoruserswidget.h
    administratordialog/users/administratoradduserdialog.h
    administratordialog/users/administratoradduserwidget.h
    administratordialog/administratorwidget.h
    administratordialog/rooms/administratorroomswidget.h
    administratordialog/rooms/administratorroomsselectroomtypewidget.h
    administratordialog/administratordialog.h
    administratordialog/logs/viewlogwidget.h
    administratordialog/invites/invitetreeview.h
    administratordialog/invites/administratorinviteswidget.h
    administratordialog/invites/administratorinvitesfilterproxymodel.h
    administratordialog/permissions/permissionswidget.h
    administratordialog/permissions/permissionseditwidget.h
    administratordialog/permissions/permissionseditdialog.h
    administratordialog/permissions/permissionstreeview.h
    administratordialog/customsounds/administratorcustomsoundscreatedialog.h
    administratordialog/customsounds/administratorcustomsoundswidget.h
    administratordialog/customsounds/administratorcustomsoundscreatewidget.h
    administratordialog/customuserstatus/administratorcustomuserstatuswidget.h
    administratordialog/customuserstatus/administratorcustomuserstatuscreatedialog.h
    administratordialog/customuserstatus/administratorcustomuserstatuscreatewidget.h
    administratordialog/customuserstatus/customuserstatustreewidget.h
    administratordialog/serverinfo/administratorserverinfowidget.h
    administratordialog/customemoji/administratorcustomemojicreatewidget.h
    administratordialog/customemoji/administratorcustomemojicreatedialog.h
    administratordialog/customemoji/administratorcustomemojiwidget.h
    administratordialog/roles/userinroleeditdialog.h
    administratordialog/roles/rolestreeview.h
    administratordialog/roles/roleeditdialog.h
    administratordialog/roles/administratorroleswidget.h
    administratordialog/roles/roleeditwidget.h
    administratordialog/roles/usersinrolewidget.h
    administratordialog/roles/rolescopecombobox.h
    libruqolawidgets_private_export.h
    prunemessages/prunemessagesdialog.h
    prunemessages/prunemessageswidget.h
    directory/directorywidget.h
    directory/directorytabwidget.h
    directory/directorydialog.h
    dialogs/searchmessagewidget.h
    dialogs/searchchannellistview.h
    dialogs/showattachmentdialog.h
    dialogs/showdiscussionswidget.h
    dialogs/channelpasswordwidget.h
    dialogs/showimagewidget.h
    dialogs/searchchanneldialog.h
    dialogs/discussion/listdiscussiondelegate.h
    dialogs/showsnipperedmessagesdialog.h
    dialogs/createnewserverwidget.h
    dialogs/createnewserverdialog.h
    dialogs/showattachmentwidget.h
    dialogs/addusersinroomwidget.h
    dialogs/channelpassworddialog.h
    dialogs/playsoundwidget.h
    dialogs/autotranslateconfigurewidget.h
    dialogs/showmentionsmessagesdialog.h
    dialogs/directchannelinfodialog.h
    dialogs/asktwoauthenticationpasswordwidget.h
    dialogs/showvideodialog.h
    dialogs/createnewdiscussiondialog.h
    dialogs/attachment/listattachmentdelegate.h
    dialogs/addusersinroomdialog.h
    dialogs/createnewchanneldialog.h
    dialogs/channelsearchnamelineresultwidget.h
    dialogs/channelinfodialog.h
    dialogs/messagetexteditor.h
    dialogs/searchmessagedialog.h
    dialogs/showvideowidget.h
    dialogs/channelsearchwidget.h
    dialogs/uploadfiledialog.h
    dialogs/createdirectmessagesdialog.h
    dialogs/createdirectmessageswidget.h
    dialogs/inviteuserswidget.h
    dialogs/showimagedialog.h
    dialogs/roomavatarwidget.h
    dialogs/channelinfoeditablewidget.h
    dialogs/uploadfilewidget.h
    dialogs/searchchannelwidget.h
    dialogs/channelinfowidget.h
    dialogs/adduserscompletionlineedit.h
    dialogs/showstarredmessagesdialog.h
    dialogs/reportmessagewidget.h
    dialogs/channelinfoprunewidget.h
    dialogs/showthreadsdialog.h
    dialogs/channelinforeadonlywidget.h
    dialogs/modifystatusdialog.h
    dialogs/configurenotificationwidget.h
    dialogs/serverinfo/serverinfowidget.h
    dialogs/serverinfo/serverinfodialog.h
    dialogs/configurenotificationdialog.h
    dialogs/channelnamevalidlineedit.h
    dialogs/showlistmessagebasewidget.h
    dialogs/channelsearchnamelineedit.h
    dialogs/inviteusersdialog.h
    dialogs/showpinnedmessagesdialog.h
    dialogs/showattachmentcombobox.h
    dialogs/createvideomessagedialog.h
    dialogs/createvideomessagewidget.h
    dialogs/searchchannel/searchchanneldelegate.h
    dialogs/autotranslateconfiguredialog.h
    dialogs/createnewchannelwidget.h
    dialogs/reportmessagedialog.h
    dialogs/playsounddialog.h
    dialogs/showlistmessagebasedialog.h
    dialogs/showdiscussionsdialog.h
    dialogs/asktwoauthenticationpassworddialog.h
    dialogs/createnewdiscussionwidget.h
    dialogs/directchannelinfowidget.h
    dialogs/modifystatuswidget.h
    teams/teamchannelscombobox.h
    teams/teamconverttochanneldialog.h
    teams/teamselectdeletedroomwidget.h
    teams/teamchannelswidget.h
    teams/addteamroomcompletionlineedit.h
    teams/teamsearchroomforteamwidget.h
    teams/searchteamcompletionlineedit.h
    teams/teamconverttochannelwidget.h
    teams/teamchannelsdialog.h
    teams/teamselectdeletedroomdialog.h
    teams/teamsearchroomdialog.h
    teams/teamsearchroomwidget.h
    teams/searchteamdialog.h
    teams/searchteamwidget.h
    ruqolacentralwidget.h

    administratordialog/rooms/administratorroomseditdialog.cpp
    administratordialog/rooms/administratorroomseditdialog.h
    administratordialog/rooms/administratorroomseditwidget.h
    administratordialog/rooms/administratorroomseditwidget.cpp
    administratordialog/rooms/administratordirectroomseditwidget.cpp
    administratordialog/rooms/administratordirectroomseditwidget.h
    administratordialog/rooms/administratorroomseditbasewidget.h
    administratordialog/rooms/administratorroomseditbasewidget.cpp
    )


ki18n_wrap_ui(libruqolawidgets
    configuredialog/configureaccountserverwidget.ui
    )



ecm_qt_declare_logging_category(libruqolawidgets_debug_SRCS HEADER ruqolawidgets_debug.h IDENTIFIER RUQOLAWIDGETS_LOG CATEGORY_NAME org.kde.ruqola.widgets
    DESCRIPTION "ruqola widgets" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolawidgets_debug_SRCS HEADER ruqolawidgets_selection_debug.h IDENTIFIER RUQOLAWIDGETS_SELECTION_LOG CATEGORY_NAME org.kde.ruqola.widgets.selection
    DESCRIPTION "ruqola widgets (selection)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolawidgets_debug_SRCS HEADER ruqolawidgets_showimage_debug.h IDENTIFIER RUQOLAWIDGETS_SHOWIMAGE_LOG CATEGORY_NAME org.kde.ruqola.widgets.showimage
    DESCRIPTION "ruqola widgets (show image dialog)" EXPORT RUQOLA)

target_sources(libruqolawidgets PRIVATE ${libruqolawidgets_debug_SRCS})

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
    add_subdirectory(otr/autotests)
endif()

install(TARGETS libruqolawidgets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
