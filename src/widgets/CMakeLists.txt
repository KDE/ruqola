# SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_library(libruqolawidgets)

if (TARGET KUserFeedbackWidgets${KUSERFEEDBACK_SUFFIX})
    target_sources(libruqolawidgets PRIVATE
        userfeedback/userfeedbackmanager.cpp
        userfeedback/ruqolauserfeedbackprovider.cpp
        userfeedback/accountinfosource.cpp
        userfeedback/ruqolauserfeedbackprovider.h
        userfeedback/accountinfosource.h
        userfeedback/userfeedbackmanager.h
    )
endif()


# Laurent port to KLineEditEventHandler in qt6
target_sources(libruqolawidgets PRIVATE
    misc/lineeditcatchreturnkey.cpp
    misc/lineeditcatchreturnkey.h
)


target_sources(libruqolawidgets PRIVATE
    administratordialog/administratordialog.cpp
    administratordialog/administratordialog.h
    administratordialog/administratorwidget.cpp
    administratordialog/administratorwidget.h
    administratordialog/customemoji/administratorcustomemojicreateorupdatedialog.cpp
    administratordialog/customemoji/administratorcustomemojicreateorupdatedialog.h
    administratordialog/customemoji/administratorcustomemojicreateorupdatewidget.cpp
    administratordialog/customemoji/administratorcustomemojicreateorupdatewidget.h
    administratordialog/customemoji/administratorcustomemojiwidget.cpp
    administratordialog/customemoji/administratorcustomemojiwidget.h
    administratordialog/customsounds/administratorcustomsoundscreatedialog.cpp
    administratordialog/customsounds/administratorcustomsoundscreatedialog.h
    administratordialog/customsounds/administratorcustomsoundscreatewidget.cpp
    administratordialog/customsounds/administratorcustomsoundscreatewidget.h
    administratordialog/customsounds/administratorcustomsoundswidget.cpp
    administratordialog/customsounds/administratorcustomsoundswidget.h
    administratordialog/customuserstatus/administratorcustomuserstatuscreatedialog.cpp
    administratordialog/customuserstatus/administratorcustomuserstatuscreatedialog.h
    administratordialog/customuserstatus/administratorcustomuserstatuscreatewidget.cpp
    administratordialog/customuserstatus/administratorcustomuserstatuscreatewidget.h
    administratordialog/customuserstatus/administratorcustomuserstatuswidget.cpp
    administratordialog/customuserstatus/administratorcustomuserstatuswidget.h
    administratordialog/customuserstatus/customuserstatustreewidget.cpp
    administratordialog/customuserstatus/customuserstatustreewidget.h
    administratordialog/invites/administratorinvitesfilterproxymodel.cpp
    administratordialog/invites/administratorinvitesfilterproxymodel.h
    administratordialog/invites/administratorinviteswidget.cpp
    administratordialog/invites/administratorinviteswidget.h
    administratordialog/invites/invitetreeview.cpp
    administratordialog/invites/invitetreeview.h
    administratordialog/logs/viewlogwidget.cpp
    administratordialog/logs/viewlogwidget.h
    administratordialog/oauth/administratoroauthcreatedialog.cpp
    administratordialog/oauth/administratoroauthcreatedialog.h
    administratordialog/oauth/administratoroauthcreatewidget.cpp
    administratordialog/oauth/administratoroauthcreatewidget.h
    administratordialog/oauth/administratoroautheditdialog.cpp
    administratordialog/oauth/administratoroautheditdialog.h
    administratordialog/oauth/administratoroautheditwidget.cpp
    administratordialog/oauth/administratoroautheditwidget.h
    administratordialog/oauth/administratoroauthfilterproxymodel.cpp
    administratordialog/oauth/administratoroauthfilterproxymodel.h
    administratordialog/oauth/administratoroauthwidget.cpp
    administratordialog/oauth/administratoroauthwidget.h
    administratordialog/oauth/oauthtreeview.cpp
    administratordialog/oauth/oauthtreeview.h
    administratordialog/permissions/permissionseditdialog.cpp
    administratordialog/permissions/permissionseditdialog.h
    administratordialog/permissions/permissionseditwidget.cpp
    administratordialog/permissions/permissionseditwidget.h
    administratordialog/permissions/permissionstreeview.cpp
    administratordialog/permissions/permissionstreeview.h
    administratordialog/permissions/permissionswidget.cpp
    administratordialog/permissions/permissionswidget.h
    administratordialog/roles/administratorroleswidget.cpp
    administratordialog/roles/administratorroleswidget.h
    administratordialog/roles/roleeditdialog.cpp
    administratordialog/roles/roleeditdialog.h
    administratordialog/roles/roleeditwidget.cpp
    administratordialog/roles/roleeditwidget.h
    administratordialog/roles/rolescopecombobox.cpp
    administratordialog/roles/rolescopecombobox.h
    administratordialog/roles/rolestreeview.cpp
    administratordialog/roles/rolestreeview.h
    administratordialog/roles/userinroleeditdialog.cpp
    administratordialog/roles/userinroleeditdialog.h
    administratordialog/roles/usersinrolewidget.cpp
    administratordialog/roles/usersinrolewidget.h
    administratordialog/rooms/administratordirectroomseditwidget.cpp
    administratordialog/rooms/administratordirectroomseditwidget.h
    administratordialog/rooms/administratorroomseditbasewidget.cpp
    administratordialog/rooms/administratorroomseditbasewidget.h
    administratordialog/rooms/administratorroomseditdialog.cpp
    administratordialog/rooms/administratorroomseditdialog.h
    administratordialog/rooms/administratorroomseditwidget.cpp
    administratordialog/rooms/administratorroomseditwidget.h
    administratordialog/rooms/administratorroomsselectroomtypewidget.cpp
    administratordialog/rooms/administratorroomsselectroomtypewidget.h
    administratordialog/rooms/administratorroomswidget.cpp
    administratordialog/rooms/administratorroomswidget.h
    administratordialog/serverinfo/administratorserverinfowidget.cpp
    administratordialog/serverinfo/administratorserverinfowidget.h
    administratordialog/users/administratoradduserdialog.cpp
    administratordialog/users/administratoradduserdialog.h
    administratordialog/users/administratoradduserwidget.cpp
    administratordialog/users/administratoradduserwidget.h
    administratordialog/users/administratoruserswidget.cpp
    administratordialog/users/administratoruserswidget.h
    administratordialog/users/administratorinviteusersdialog.h
    administratordialog/users/administratorinviteusersdialog.cpp
    administratordialog/users/administratorinviteuserswidget.h
    administratordialog/users/administratorinviteuserswidget.cpp

    administratorsettingsdialog/administratorsettingsdialog.cpp
    administratorsettingsdialog/administratorsettingsdialog.h
    administratorsettingsdialog/administratorsettingswidget.h
    administratorsettingsdialog/administratorsettingswidget.cpp
    administratorsettingsdialog/accounts/accountsettingswidget.h
    administratorsettingsdialog/accounts/accountsettingswidget.cpp
    administratorsettingsdialog/encryption/encryptionsettingswidget.h
    administratorsettingsdialog/encryption/encryptionsettingswidget.cpp
    administratorsettingsdialog/message/messagesettingswidget.cpp
    administratorsettingsdialog/message/messagesettingswidget.h
    administratorsettingsdialog/settingswidgetbase.h
    administratorsettingsdialog/settingswidgetbase.cpp
    administratorsettingsdialog/fileupload/fileuploadsettingswidget.h
    administratorsettingsdialog/fileupload/fileuploadsettingswidget.cpp
    administratorsettingsdialog/retentionpolicy/retentionpolicysettingswidget.h
    administratorsettingsdialog/retentionpolicy/retentionpolicysettingswidget.cpp
    administratorsettingsdialog/general/generalsettingswidget.h
    administratorsettingsdialog/general/generalsettingswidget.cpp

    administratorsettingsdialog/ratelimiter/ratelimiterwidget.cpp
    administratorsettingsdialog/ratelimiter/ratelimiterwidget.h

    administratorsettingsdialog/password/passwordsettingswidget.h
    administratorsettingsdialog/password/passwordsettingswidget.cpp

    administratorsettingsdialog/videoconference/videoconferencewidget.cpp
    administratorsettingsdialog/videoconference/videoconferencewidget.h

    administratorsettingsdialog/ircfederation/ircfederationwidget.h
    administratorsettingsdialog/ircfederation/ircfederationwidget.cpp

    administratorsettingsdialog/irc/ircwidget.h
    administratorsettingsdialog/irc/ircwidget.cpp

    administratorsettingsdialog/webdav/webdavsettingswidget.h
    administratorsettingsdialog/webdav/webdavsettingswidget.cpp

    administratorsettingsdialog/ldap/ldapsettingswidget.h
    administratorsettingsdialog/ldap/ldapsettingswidget.cpp

    administratorsettingsdialog/layout/layoutsettingswidget.cpp
    administratorsettingsdialog/layout/layoutsettingswidget.h

    administratorsettingsdialog/enterprise/enterprisesettingswidget.h
    administratorsettingsdialog/enterprise/enterprisesettingswidget.cpp

    administratorsettingsdialog/userdatadownload/userdatadownloadwidget.h
    administratorsettingsdialog/userdatadownload/userdatadownloadwidget.cpp

    administratorsettingsdialog/slackbridge/slackbridgewidget.cpp
    administratorsettingsdialog/slackbridge/slackbridgewidget.h

    administratorsettingsdialog/logs/logssettingswidget.h
    administratorsettingsdialog/logs/logssettingswidget.cpp

    administratorsettingsdialog/email/emailsettingswidget.h
    administratorsettingsdialog/email/emailsettingswidget.cpp

    administratorsettingsdialog/mobile/mobilesettingswidget.h
    administratorsettingsdialog/mobile/mobilesettingswidget.cpp

    administratorsettingsdialog/troubleshoot/troubleshootsettingswidget.h
    administratorsettingsdialog/troubleshoot/troubleshootsettingswidget.cpp

    administratorsettingsdialog/conferencecall/conferencecallsettingswidget.cpp
    administratorsettingsdialog/conferencecall/conferencecallsettingswidget.h

    administratorsettingsdialog/webrtc/webrtcsettingswidget.cpp
    administratorsettingsdialog/webrtc/webrtcsettingswidget.h

    administratorsettingsdialog/cas/cassettingswidget.h
    administratorsettingsdialog/cas/cassettingswidget.cpp

    administratorsettingsdialog/oauth/oauthsettingswidget.h
    administratorsettingsdialog/oauth/oauthsettingswidget.cpp

    administratorsettingsdialog/analytics/analyticswidget.cpp
    administratorsettingsdialog/analytics/analyticswidget.h

    administratordialog/logs/viewlogplaintextedit.h
    administratordialog/logs/viewlogplaintextedit.cpp

    bannerinfodialog/bannerinfodialog.h
    bannerinfodialog/bannerinfodialog.cpp
    bannerinfodialog/bannerinfowidget.h
    bannerinfodialog/bannerinfowidget.cpp
    bannerinfodialog/bannerinfolistview.h
    bannerinfodialog/bannerinfolistview.cpp
    bannerinfodialog/bannerinfolistsearchlinewidget.h
    bannerinfodialog/bannerinfolistsearchlinewidget.cpp
    bannerinfodialog/bannerinfolistviewdelegate.h
    bannerinfodialog/bannerinfolistviewdelegate.cpp

    bannerinfodialog/bannermessagewidget.h
    bannerinfodialog/bannermessagewidget.cpp

    channellist/channellistdelegate.cpp
    channellist/channellistdelegate.h
    channellist/channellistview.cpp
    channellist/channellistview.h
    channellist/channellistwidget.cpp
    channellist/channellistwidget.h
    common/authenticationcombobox.cpp
    common/authenticationcombobox.h
    common/commandcompletiondelegate.cpp
    common/commandcompletiondelegate.h
    common/completionlineedit.cpp
    common/completionlineedit.h
    common/completionlistview.cpp
    common/completionlistview.h
    common/delegatepaintutil.cpp
    common/delegatepaintutil.h
    common/delegateutil.cpp
    common/delegateutil.h
    common/emojicompletiondelegate.cpp
    common/emojicompletiondelegate.h
    common/flowlayout.cpp
    common/flowlayout.h
    configuredialog/accountserverlistwidget.cpp
    configuredialog/accountserverlistwidget.h
    configuredialog/configureaccountserverwidget.cpp
    configuredialog/configureaccountserverwidget.h
    configuredialog/configureaccountwidget.cpp
    configuredialog/configureaccountwidget.h
    configuredialog/configurefontwidget.cpp
    configuredialog/configurefontwidget.h
    configuredialog/configuregeneralwidget.cpp
    configuredialog/configuregeneralwidget.h
    configuredialog/configuresettingsdialog.cpp
    configuredialog/configuresettingsdialog.h
    configuredialog/configurespellcheckingwidget.cpp
    configuredialog/configurespellcheckingwidget.h
    configuredialog/configureuserfeedbackwidget.cpp
    configuredialog/configureuserfeedbackwidget.h
    configuredialog/removeaccountdialog.cpp
    configuredialog/removeaccountdialog.h
    configuredialog/removeaccountwidget.cpp
    configuredialog/removeaccountwidget.h


    delegateutils/messagedelegateutils.cpp
    delegateutils/messagedelegateutils.h
    delegateutils/textselection.cpp
    delegateutils/textselection.h
    delegateutils/textselectionimpl.h
    delegateutils/textselectionimpl.cpp
    dialogs/adduserscompletionlineedit.cpp
    dialogs/adduserscompletionlineedit.h
    dialogs/addusersinroomdialog.cpp
    dialogs/addusersinroomdialog.h
    dialogs/addusersinroomwidget.cpp
    dialogs/addusersinroomwidget.h
    dialogs/asktwoauthenticationpassworddialog.cpp
    dialogs/asktwoauthenticationpassworddialog.h
    dialogs/asktwoauthenticationpasswordwidget.cpp
    dialogs/asktwoauthenticationpasswordwidget.h
    dialogs/attachment/listattachmentdelegate.cpp
    dialogs/attachment/listattachmentdelegate.h
    dialogs/autotranslateconfiguredialog.cpp
    dialogs/autotranslateconfiguredialog.h
    dialogs/autotranslateconfigurewidget.cpp
    dialogs/autotranslateconfigurewidget.h
    dialogs/channelinfodialog.cpp
    dialogs/channelinfodialog.h
    dialogs/channelinfoeditablewidget.cpp
    dialogs/channelinfoeditablewidget.h
    dialogs/channelinfoprunewidget.cpp
    dialogs/channelinfoprunewidget.h
    dialogs/channelinforeadonlywidget.cpp
    dialogs/channelinforeadonlywidget.h
    dialogs/channelinfowidget.cpp
    dialogs/channelinfowidget.h
    dialogs/channelnamevalidlineedit.cpp
    dialogs/channelnamevalidlineedit.h
    dialogs/channelnamevalidlinewidget.cpp
    dialogs/channelnamevalidlinewidget.h
    dialogs/channelpassworddialog.cpp
    dialogs/channelpassworddialog.h
    dialogs/channelpasswordwidget.cpp
    dialogs/channelpasswordwidget.h
    dialogs/channelsearchnamelineedit.cpp
    dialogs/channelsearchnamelineedit.h
    dialogs/channelsearchnamelineresultwidget.cpp
    dialogs/channelsearchnamelineresultwidget.h
    dialogs/channelsearchwidget.cpp
    dialogs/channelsearchwidget.h
    dialogs/configurenotificationdialog.cpp
    dialogs/configurenotificationdialog.h
    dialogs/configurenotificationwidget.cpp
    dialogs/configurenotificationwidget.h
    dialogs/confirmpassworddialog.cpp
    dialogs/confirmpassworddialog.h
    dialogs/confirmpasswordwidget.cpp
    dialogs/confirmpasswordwidget.h
    dialogs/createdirectmessagesdialog.cpp
    dialogs/createdirectmessagesdialog.h
    dialogs/createdirectmessageswidget.cpp
    dialogs/createdirectmessageswidget.h
    dialogs/createnewchanneldialog.cpp
    dialogs/createnewchanneldialog.h
    dialogs/createnewchannelwidget.cpp
    dialogs/createnewchannelwidget.h
    dialogs/createnewdiscussiondialog.cpp
    dialogs/createnewdiscussiondialog.h
    dialogs/createnewdiscussionwidget.cpp
    dialogs/createnewdiscussionwidget.h
    dialogs/createnewserverdialog.cpp
    dialogs/createnewserverdialog.h
    dialogs/createnewserverwidget.cpp
    dialogs/createnewserverwidget.h


    dialogs/directchannelinfodialog.cpp
    dialogs/directchannelinfodialog.h
    dialogs/directchannelinfowidget.cpp
    dialogs/directchannelinfowidget.h
    dialogs/inviteusersdialog.cpp
    dialogs/inviteusersdialog.h
    dialogs/inviteuserswidget.cpp
    dialogs/inviteuserswidget.h
    dialogs/messagetexteditor.cpp
    dialogs/messagetexteditor.h
    dialogs/modifystatusdialog.cpp
    dialogs/modifystatusdialog.h
    dialogs/modifystatuswidget.cpp
    dialogs/modifystatuswidget.h
    dialogs/playsounddialog.cpp
    dialogs/playsounddialog.h
    dialogs/playsoundwidget.cpp
    dialogs/playsoundwidget.h
    dialogs/reportmessagedialog.cpp
    dialogs/reportmessagedialog.h
    dialogs/reportmessagewidget.cpp
    dialogs/reportmessagewidget.h
    dialogs/roomavatarwidget.cpp
    dialogs/roomavatarwidget.h
    dialogs/roomavatarreadonlywidget.h
    dialogs/roomavatarreadonlywidget.cpp
    dialogs/searchmessagedialog.cpp
    dialogs/searchmessagedialog.h
    dialogs/searchmessagewidget.cpp
    dialogs/searchmessagewidget.h
    dialogs/searchmessagewithdelaylineedit.cpp
    dialogs/searchmessagewithdelaylineedit.h
    dialogs/serverinfo/serverinfodialog.cpp
    dialogs/serverinfo/serverinfodialog.h
    dialogs/serverinfo/serverinfowidget.cpp
    dialogs/serverinfo/serverinfowidget.h
    dialogs/showattachmentcombobox.cpp
    dialogs/showattachmentcombobox.h
    dialogs/showattachmentdialog.cpp
    dialogs/showattachmentdialog.h
    dialogs/showattachmentwidget.cpp
    dialogs/showattachmentwidget.h
    dialogs/showimagedialog.cpp
    dialogs/showimagedialog.h
    dialogs/showimagewidget.cpp
    dialogs/showimagewidget.h
    dialogs/showlistmessagebasedialog.cpp
    dialogs/showlistmessagebasedialog.h
    dialogs/showlistmessagebasewidget.cpp
    dialogs/showlistmessagebasewidget.h
    dialogs/showmentionsmessagesdialog.cpp
    dialogs/showmentionsmessagesdialog.h
    dialogs/showpinnedmessagesdialog.cpp
    dialogs/showpinnedmessagesdialog.h
    dialogs/showstarredmessagesdialog.cpp
    dialogs/showstarredmessagesdialog.h
    dialogs/showthreadsdialog.cpp
    dialogs/showthreadsdialog.h
    dialogs/showvideodialog.cpp
    dialogs/showvideodialog.h
    dialogs/showvideowidget.cpp
    dialogs/showvideowidget.h
    dialogs/uploadfiledialog.cpp
    dialogs/uploadfiledialog.h
    dialogs/uploadfilewidget.cpp
    dialogs/uploadfilewidget.h
    dialogs/channelrolesinfowidget.h
    dialogs/channelrolesinfowidget.cpp

    discussions/showdiscussionsdialog.cpp
    discussions/showdiscussionsdialog.h
    discussions/showdiscussionswidget.cpp
    discussions/showdiscussionswidget.h
    discussions/discussionlistview.h
    discussions/discussionlistview.cpp
    discussions/discussion/listdiscussiondelegate.cpp
    discussions/discussion/listdiscussiondelegate.h
    directory/directorydialog.cpp
    directory/directorydialog.h
    directory/directorytabwidget.cpp
    directory/directorytabwidget.h
    directory/directorywidget.cpp
    directory/directorywidget.h

    directory/directorystackedwidget.h
    directory/directorystackedwidget.cpp

    directory/directorynotauthorizedwidget.cpp
    directory/directorynotauthorizedwidget.h

    exportmessages/exportmessagesdialog.cpp
    exportmessages/exportmessagesdialog.h
    exportmessages/exportmessageswidget.cpp
    exportmessages/exportmessageswidget.h
    libruqolawidgets_private_export.h
    messagemaximumsizedialog/messagemaximumsizedialog.cpp
    messagemaximumsizedialog/messagemaximumsizedialog.h
    messagemaximumsizedialog/messagemaximumsizewidget.cpp
    messagemaximumsizedialog/messagemaximumsizewidget.h
    misc/accountsoverviewwidget.cpp
    misc/accountsoverviewwidget.h
    misc/adduserswidget.cpp
    misc/adduserswidget.h
    misc/avatarcachemanager.cpp
    misc/avatarcachemanager.h
    misc/clickablewidget.cpp
    misc/clickablewidget.h
    misc/emoticonmenuwidget.cpp
    misc/emoticonmenuwidget.h
    misc/messagelistviewbase.h
    misc/messagelistviewbase.cpp
    misc/passwordconfirmwidget.cpp
    misc/passwordconfirmwidget.h
    misc/passwordlineeditwidget.cpp
    misc/passwordlineeditwidget.h
    misc/pixmapcache.cpp
    misc/pixmapcache.h
    misc/rolescombobox.cpp
    misc/rolescombobox.h
    misc/searchtreebasewidget.cpp
    misc/searchtreebasewidget.h
    misc/searchwithdelaylineedit.cpp
    misc/searchwithdelaylineedit.h
    misc/servermenu.cpp
    misc/servermenu.h
    misc/statuscombobox.cpp
    misc/statuscombobox.h
    misc/systemmessagescombobox.cpp
    misc/systemmessagescombobox.h
    misc/twoauthenticationpasswordwidget.cpp
    misc/twoauthenticationpasswordwidget.h
    misc/messagelistdelegatebase.cpp
    misc/messagelistdelegatebase.h
    misc/serverscombobox.h
    misc/serverscombobox.cpp

    myaccount/myaccount2e2configurewidget.cpp
    myaccount/myaccount2e2configurewidget.h
    myaccount/myaccount2faconfigurewidget.cpp
    myaccount/myaccount2faconfigurewidget.h
    myaccount/myaccount2fadisabletotpwidget.cpp
    myaccount/myaccount2fadisabletotpwidget.h
    myaccount/myaccount2fatotpwidget.cpp
    myaccount/myaccount2fatotpwidget.h
    myaccount/myaccountconfiguredialog.cpp
    myaccount/myaccountconfiguredialog.h
    myaccount/myaccountconfigurewidget.cpp
    myaccount/myaccountconfigurewidget.h
    myaccount/myaccountpreferenceconfigurewidget.cpp
    myaccount/myaccountpreferenceconfigurewidget.h
    myaccount/myaccountprofileconfigureavatarwidget.cpp
    myaccount/myaccountprofileconfigureavatarwidget.h
    myaccount/myaccountprofileconfigurewidget.cpp
    myaccount/myaccountprofileconfigurewidget.h

    myaccount/myaccountpersonalaccesstokenconfigurewidget.h
    myaccount/myaccountpersonalaccesstokenconfigurewidget.cpp
    myaccount/myaccountpersonalaccesstokentreeview.h
    myaccount/myaccountpersonalaccesstokentreeview.cpp
    myaccount/myaccountpersonalaccesscreatedialog.h
    myaccount/myaccountpersonalaccesscreatedialog.cpp
    myaccount/myaccountpersonalaccesscreatewidget.h
    myaccount/myaccountpersonalaccesscreatewidget.cpp

    myaccount/myaccountmanagedeviceconfigurewidget.cpp
    myaccount/myaccountmanagedeviceconfigurewidget.h

    notificationhistory/notificationhistorydialog.cpp
    notificationhistory/notificationhistorydialog.h
    notificationhistory/notificationhistorywidget.cpp
    notificationhistory/notificationhistorywidget.h
    notificationhistory/notificationhistorydelegate.h
    notificationhistory/notificationhistorydelegate.cpp
    notificationhistory/notificationhistorylistview.h
    notificationhistory/notificationhistorylistview.cpp
    otr/otrwidget.cpp
    otr/otrwidget.h
    prunemessages/prunemessagesdialog.cpp
    prunemessages/prunemessagesdialog.h
    prunemessages/prunemessageswidget.cpp
    prunemessages/prunemessageswidget.h
    registeruser/registeruserdialog.cpp
    registeruser/registeruserdialog.h
    registeruser/registeruserwidget.cpp
    registeruser/registeruserwidget.h
    room/channelactionpopupmenu.cpp
    room/channelactionpopupmenu.h

    room/delegate/messagedelegatehelperbase.h
    room/delegate/messagedelegatehelperbase.cpp

    room/delegate/messageattachmentdelegatehelperfile.cpp
    room/delegate/messageattachmentdelegatehelperfile.h
    room/delegate/messageattachmentdelegatehelperimage.cpp
    room/delegate/messageattachmentdelegatehelperimage.h
    room/delegate/messageattachmentdelegatehelpersound.cpp
    room/delegate/messageattachmentdelegatehelpersound.h
    room/delegate/messageattachmentdelegatehelpertext.cpp
    room/delegate/messageattachmentdelegatehelpertext.h
    room/delegate/messageattachmentdelegatehelpervideo.cpp
    room/delegate/messageattachmentdelegatehelpervideo.h
    room/delegate/messagedelegatehelperconferencevideo.h
    room/delegate/messagedelegatehelperconferencevideo.cpp

    room/delegate/messageimagedownloadjob.h
    room/delegate/messageimagedownloadjob.cpp

    room/delegate/messageblockdelegatehelperbase.h
    room/delegate/messageblockdelegatehelperbase.cpp

    room/delegate/messageattachmentdelegatehelperbase.cpp
    room/delegate/messageattachmentdelegatehelperbase.h
    room/delegate/messagedelegatehelperreactions.cpp
    room/delegate/messagedelegatehelperreactions.h
    room/delegate/messagedelegatehelpertext.cpp
    room/delegate/messagedelegatehelpertext.h
    room/delegate/messagelistdelegate.cpp
    room/delegate/messagelistdelegate.h
    room/delegate/runninganimatedimage.cpp
    room/delegate/runninganimatedimage.h
    room/messagelinewidget.cpp
    room/messagelinewidget.h
    room/messagelistview.cpp
    room/messagelistview.h
    room/messagetextedit.cpp
    room/messagetextedit.h
    room/plugins/plugintext.cpp
    room/plugins/plugintext.h
    room/plugins/plugintextinterface.cpp
    room/plugins/plugintextinterface.h
    room/readonlylineeditwidget.cpp
    room/readonlylineeditwidget.h
    room/reconnectinfowidget.cpp
    room/reconnectinfowidget.h
    room/roomcounterinfowidget.cpp
    room/roomcounterinfowidget.h
    room/roomheaderlabel.cpp
    room/roomheaderlabel.h
    room/roomheaderwidget.cpp
    room/roomheaderwidget.h
    room/roomquotemessagewidget.cpp
    room/roomquotemessagewidget.h
    room/roomreplythreadwidget.cpp
    room/roomreplythreadwidget.h
    room/roomutil.cpp
    room/roomutil.h
    room/roomwidgetbase.cpp
    room/roomwidgetbase.h
    room/roomwidget.cpp
    room/roomwidget.h
    room/teamnamelabel.cpp
    room/teamnamelabel.h
    room/textpluginmanager.cpp
    room/textpluginmanager.h
    room/uploadfileprogressstatuslistwidget.cpp
    room/uploadfileprogressstatuslistwidget.h
    room/uploadfileprogressstatuswidget.cpp
    room/uploadfileprogressstatuswidget.h
    room/usersinroomcombobox.cpp
    room/usersinroomcombobox.h
    room/usersinroomdialog.cpp
    room/usersinroomdialog.h
    room/usersinroomflowwidget.cpp
    room/usersinroomflowwidget.h
    room/usersinroomlabel.cpp
    room/usersinroomlabel.h
    room/usersinroommenu.cpp
    room/usersinroommenu.h
    room/usersinroomwidget.cpp
    room/usersinroomwidget.h

    room/selectedmessagebackgroundanimation.h
    room/selectedmessagebackgroundanimation.cpp

    room/plugintextmessagewidget.h
    room/plugintextmessagewidget.cpp

    ruqolacentralwidget.cpp
    ruqolacentralwidget.h
    ruqolacommandlineoptions.cpp
    ruqolacommandlineoptions.h
    ruqolaloginwidget.cpp
    ruqolaloginwidget.h
    ruqolamainwidget.cpp
    ruqolamainwidget.h
    ruqolamainwindow.cpp
    ruqolamainwindow.h
    switchchannelhistory/switchchanneltreeview.cpp
    switchchannelhistory/switchchanneltreeview.h
    switchchannelhistory/switchchanneltreeviewmanager.cpp
    switchchannelhistory/switchchanneltreeviewmanager.h
    teams/addteamroomcompletionlineedit.cpp
    teams/addteamroomcompletionlineedit.h
    teams/searchteamcompletionlineedit.cpp
    teams/searchteamcompletionlineedit.h
    teams/searchteamdialog.cpp
    teams/searchteamdialog.h
    teams/searchteamwidget.cpp
    teams/searchteamwidget.h
    teams/teamchannelscombobox.cpp
    teams/teamchannelscombobox.h
    teams/teamchannelsdialog.cpp
    teams/teamchannelsdialog.h
    teams/teamchannelswidget.cpp
    teams/teamchannelswidget.h
    teams/teamconverttochanneldialog.cpp
    teams/teamconverttochanneldialog.h
    teams/teamconverttochannelwidget.cpp
    teams/teamconverttochannelwidget.h
    teams/teamsearchroomdialog.cpp
    teams/teamsearchroomdialog.h
    teams/teamsearchroomforteamwidget.cpp
    teams/teamsearchroomforteamwidget.h
    teams/teamsearchroomwidget.cpp
    teams/teamsearchroomwidget.h
    teams/teamselectdeletedroomdialog.cpp
    teams/teamselectdeletedroomdialog.h
    teams/teamselectdeletedroomwidget.cpp
    teams/teamselectdeletedroomwidget.h
    threadwidget/threadmessagedialog.cpp
    threadwidget/threadmessagedialog.h
    threadwidget/threadmessagewidget.cpp
    threadwidget/threadmessagewidget.h

    ruqolawidget.qrc

    room/delegate/messagelistlayout/messagelistlayoutbase.h
    room/delegate/messagelistlayout/messagelistlayoutbase.cpp

    room/delegate/messagelistlayout/messagelistcompactlayout.h
    room/delegate/messagelistlayout/messagelistcompactlayout.cpp

    room/delegate/messagelistlayout/messagelistnormallayout.h
    room/delegate/messagelistlayout/messagelistnormallayout.cpp

    room/delegate/messagelistlayout/messagelistcozylayout.h
    room/delegate/messagelistlayout/messagelistcozylayout.cpp

    conferencecalldialog/conferencecalldialog.h
    conferencecalldialog/conferencecalldialog.cpp
    conferencecalldialog/conferencecallwidget.h
    conferencecalldialog/conferencecallwidget.cpp
    conferencecalldialog/conferencedirectcalldialog.h
    conferencecalldialog/conferencedirectcalldialog.cpp

    conferencecalldialog/conferenceinfodialog.h
    conferencecalldialog/conferenceinfodialog.cpp
    conferencecalldialog/conferenceinfowidget.h
    conferencecalldialog/conferenceinfowidget.cpp

    servererrorinfohistory/servererrorinfomessagewidget.h
    servererrorinfohistory/servererrorinfomessagewidget.cpp
    servererrorinfohistory/servererrorinfomessagehistorydialog.h
    servererrorinfohistory/servererrorinfomessagehistorydialog.cpp

    servererrorinfohistory/servererrorinfomessagehistorywidget.h
    servererrorinfohistory/servererrorinfomessagehistorywidget.cpp

    servererrorinfohistory/servererrorinfomessagehistorylistview.h
    servererrorinfohistory/servererrorinfomessagehistorylistview.cpp
    servererrorinfohistory/servererrorinfohistorydelegate.h
    servererrorinfohistory/servererrorinfohistorydelegate.cpp

    databasedialog/exploredatabasedialog.h
    databasedialog/exploredatabasedialog.cpp
    databasedialog/exploredatabasewidget.h
    databasedialog/exploredatabasewidget.cpp


    whatsnew/whatsnewdialog.h
    whatsnew/whatsnewdialog.cpp
    whatsnew/whatsnewwidget.h
    whatsnew/whatsnewwidget.cpp

    whatsnew/whatsnewmessagewidget.h
    whatsnew/whatsnewmessagewidget.cpp

    whatsnew/whatsnewcomboboxwidget.h
    whatsnew/whatsnewcomboboxwidget.cpp

    explorepermissionsdialog/explorepermissionsdialog.h
    explorepermissionsdialog/explorepermissionsdialog.cpp
    explorepermissionsdialog/explorepermissionswidget.h
    explorepermissionsdialog/explorepermissionswidget.cpp
    explorepermissionsdialog/explorepermissionswidgetfilterproxymodel.cpp
    explorepermissionsdialog/explorepermissionswidgetfilterproxymodel.h


    importexportdata/exportdata/exportdatawizard.h
    importexportdata/exportdata/exportdatawizard.cpp
    importexportdata/exportdata/exportdataselectaccountpage.h
    importexportdata/exportdata/exportdataselectaccountpage.cpp
    importexportdata/exportdata/exportdatafinishpage.h
    importexportdata/exportdata/exportdatafinishpage.cpp
    importexportdata/exportdata/exportdataselectaccountlistwidget.h
    importexportdata/exportdata/exportdataselectaccountlistwidget.cpp
    importexportdata/exportdata/exportaccountjob.h
    importexportdata/exportdata/exportaccountjob.cpp

    importexportdata/importdata/importdatawizard.h
    importexportdata/importdata/importdatawizard.cpp
    importexportdata/importdata/importdataselectaccountpage.h
    importexportdata/importdata/importdataselectaccountpage.cpp
    importexportdata/importdata/importdatafinishpage.h
    importexportdata/importdata/importdatafinishpage.cpp
    importexportdata/importdata/importaccountjob.h
    importexportdata/importdata/importaccountjob.cpp

    importexportdata/importexportutils.h
    importexportdata/importexportutils.cpp

    offlinewidget/offlinewidget.h
    offlinewidget/offlinewidget.cpp
)

if (HAVE_TEXT_AUTOCORRECTION)
    target_sources(libruqolawidgets PRIVATE
        configuredialog/configureautocorrectionwidget.h
        configuredialog/configureautocorrectionwidget.cpp
    )
endif()
if (HAVE_TEXT_AUTOCORRECTION_WIDGETS)
    target_sources(libruqolawidgets PRIVATE
        configuredialog/configureautocorrectionwidget.h
        configuredialog/configureautocorrectionwidget.cpp
    )
endif()

if (QT_MAJOR_VERSION STREQUAL "6")
    target_sources(libruqolawidgets PRIVATE
        dialogs/createvideomessagewidget.cpp
        dialogs/createvideomessagewidget.h
        dialogs/createsoundmessagewidget.h
        dialogs/createsoundmessagewidget.cpp

        dialogs/createvideomessagewizard.h
        dialogs/createvideomessagewizard.cpp

        dialogs/createsoundmessagewizard.h
        dialogs/createsoundmessagewizard.cpp
    )
endif()

if (HAVE_TEXT_TO_SPEECH)
    target_sources(libruqolawidgets PRIVATE
        configuredialog/configureaccessibilitywidget.h
        configuredialog/configureaccessibilitywidget.cpp
    )
endif()

ki18n_wrap_ui(libruqolawidgets
    configuredialog/configureaccountserverwidget.ui
)

ecm_qt_declare_logging_category(libruqolawidgets_debug_SRCS HEADER ruqolawidgets_debug.h IDENTIFIER RUQOLAWIDGETS_LOG CATEGORY_NAME org.kde.ruqola.widgets
    DESCRIPTION "ruqola widgets" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolawidgets_debug_SRCS HEADER ruqolawidgets_selection_debug.h IDENTIFIER RUQOLAWIDGETS_SELECTION_LOG CATEGORY_NAME org.kde.ruqola.widgets.selection
    DESCRIPTION "ruqola widgets (selection)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolawidgets_debug_SRCS HEADER ruqolawidgets_showimage_debug.h IDENTIFIER RUQOLAWIDGETS_SHOWIMAGE_LOG CATEGORY_NAME org.kde.ruqola.widgets.showimage
    DESCRIPTION "ruqola widgets (show image dialog)" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolawidgets_debug_SRCS HEADER ruqola_thread_message_widgets_debug.h IDENTIFIER RUQOLA_THREAD_MESSAGE_WIDGETS_LOG CATEGORY_NAME org.kde.ruqola.threadmessages
    DESCRIPTION "ruqola thread message widgets" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolawidgets_debug_SRCS HEADER ruqola_password_widgets_debug.h IDENTIFIER RUQOLA_PASSWORD_WIDGETS_LOG CATEGORY_NAME org.kde.ruqola.widgets.password
    DESCRIPTION "ruqola password widget" EXPORT RUQOLA)
ecm_qt_declare_logging_category(libruqolawidgets_debug_SRCS HEADER ruqola_importexport_accounts_debug.h IDENTIFIER RUQOLA_IMPORT_EXPORT_ACCOUNTS_LOG CATEGORY_NAME org.kde.ruqola.widgets.importexportaccounts
    DESCRIPTION "ruqola import export account" EXPORT RUQOLA)

target_sources(libruqolawidgets PRIVATE ${libruqolawidgets_debug_SRCS})

ruqola_target_precompile_headers(libruqolawidgets PUBLIC ../../ruqola_pch.h)
if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(libruqolawidgets PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(libruqolawidgets BASE_NAME libruqolawidgets)

if (TARGET KUserFeedbackWidgets${KUSERFEEDBACK_SUFFIX})
    target_link_libraries(libruqolawidgets KUserFeedbackWidgets${KUSERFEEDBACK_SUFFIX})
endif()

target_link_libraries(libruqolawidgets
    Qt::Gui
    Qt::Widgets
    Qt::MultimediaWidgets
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::ConfigCore
    KF${KF_MAJOR_VERSION}::XmlGui
    KF${KF_MAJOR_VERSION}::KIOWidgets
    KF${KF_MAJOR_VERSION}::WidgetsAddons
    KF${KF_MAJOR_VERSION}::SonnetUi
    KF${KF_MAJOR_VERSION}::TextWidgets
    KF${KF_MAJOR_VERSION}::NotifyConfig
    KF${KF_MAJOR_VERSION}::ItemViews
    KF${KF_MAJOR_VERSION}::Prison
    KF${KF_MAJOR_VERSION}::KIOFileWidgets
    KF${KF_MAJOR_VERSION}::Archive
    librocketchatrestapi-qt5
    libruqolacore
)

if (HAVE_TEXT_TRANSLATOR)
    target_link_libraries(libruqolawidgets
        KF${KF_MAJOR_VERSION}::TextTranslator
    )
endif()

if (TARGET KF${KF_MAJOR_VERSION}::TextEmoticonsWidgets)
	target_link_libraries(libruqolawidgets
		KF${KF_MAJOR_VERSION}::TextEmoticonsWidgets)
endif()	

if (HAVE_TEXT_AUTOCORRECTION)
    target_link_libraries(libruqolawidgets
        KF${KF_MAJOR_VERSION}::TextAutoCorrection
    )
endif()
if (HAVE_TEXT_AUTOCORRECTION_WIDGETS)
    target_link_libraries(libruqolawidgets
        KF${KF_MAJOR_VERSION}::TextAutoCorrectionWidgets
    )
endif()

if (HAVE_TEXT_TO_SPEECH)
    target_link_libraries(libruqolawidgets
        KF${KF_MAJOR_VERSION}::TextEditTextToSpeech
    )
endif()

if (HAVE_TEXT_CUSTOM_EDITOR)
    target_link_libraries(libruqolawidgets
        KF${KF_MAJOR_VERSION}::TextCustomEditor
    )
endif()

if (NOT WIN32 AND NOT APPLE)
    target_link_libraries(libruqolawidgets KF${KF_MAJOR_VERSION}::WindowSystem)
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
    add_subdirectory(otr/autotests)
    add_subdirectory(switchchannelhistory/autotests)
    add_subdirectory(messagemaximumsizedialog/autotests)
    add_subdirectory(notificationhistory/autotests)
    add_subdirectory(discussions/autotests/)
    add_subdirectory(administratorsettingsdialog/autotests/)
    add_subdirectory(bannerinfodialog/autotests)
    add_subdirectory(conferencecalldialog/autotests)
    add_subdirectory(servererrorinfohistory/autotests)
    add_subdirectory(databasedialog/autotests)
    add_subdirectory(whatsnew/autotests)
    add_subdirectory(explorepermissionsdialog/autotests)
    add_subdirectory(importexportdata/autotests)
    add_subdirectory(importexportdata/tests)
    add_subdirectory(offlinewidget/autotests)
endif()

install(TARGETS libruqolawidgets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
