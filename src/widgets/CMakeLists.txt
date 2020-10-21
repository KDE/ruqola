set(ruqola_userfeedback_SRCS)
if (TARGET KUserFeedbackWidgets)
    set(ruqola_userfeedback_SRCS
        userfeedback/userfeedbackmanager.cpp
        userfeedback/ruqolauserfeedbackprovider.cpp
        userfeedback/accountinfosource.cpp
        )
endif()


set(Ruqola_widgets_SRCS
    ruqolamainwindow.cpp
    ruqolacentralwidget.cpp
    ruqolaloginwidget.cpp
    ruqolamainwidget.cpp
    )

set(Ruqola_widget_register_user_SRCS
    registeruser/registeruserdialog.cpp
    registeruser/registeruserwidget.cpp
    )

set(Ruqola_widgets_dialog_SRCS
    dialogs/serverinfowidget.cpp
    dialogs/serverinfodialog.cpp
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
    dialogs/createnewaccountdialog.cpp
    dialogs/createnewaccountwidget.cpp
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
    )

set(Ruqola_configure_SRCS
    configuredialog/configuresettingsdialog.cpp
    configuredialog/configureaccountwidget.cpp
    configuredialog/accountserverlistwidget.cpp
    configuredialog/configureaccountserverwidget.cpp
    configuredialog/configurespellcheckingwidget.cpp
    configuredialog/configureuserfeedbackwidget.cpp
    configuredialog/configuregeneralwidget.cpp
    configuredialog/configurefontwidget.cpp
    )

set(Ruqola_threadwidget_SRCS
    threadwidget/threadmessagewidget.cpp
    threadwidget/threadmessagedialog.cpp
    )

ki18n_wrap_ui(Ruqola_configure_SRCS
    configuredialog/configureaccountserverwidget.ui
    )


set(Ruqola_channellist_SRCS
    channellist/channellistwidget.cpp
    channellist/statuscombobox.cpp
    channellist/channellistview.cpp
    channellist/channellistdelegate.cpp
    )
set(Ruqola_prunemessages_SRCS
    prunemessages/prunemessagesdialog.cpp
    prunemessages/prunemessageswidget.cpp
    )

set(Ruqola_roomwidget_SRCS
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
    )

set(Ruqola_misc_widget_SRCS
    misc/pixmapcache.cpp
    misc/avatarcachemanager.cpp
    misc/accountmenu.cpp
    misc/accountsoverviewwidget.cpp
    misc/emoticonmenuwidget.cpp
    misc/emoticonselectorwidget.cpp
    misc/emoticonrecentusedfilterproxymodel.cpp
    misc/adduserswidget.cpp
    misc/clickableuserwidget.cpp
    misc/recentusedemoticonview.cpp
    misc/lineeditcatchreturnkey.cpp
    misc/passwordlineeditwidget.cpp
    misc/passwordconfirmwidget.cpp
    misc/twoauthenticationpasswordwidget.cpp
    )

set(Ruqola_common_SRCS
    common/completionlineedit.cpp
    common/completionlistview.cpp
    common/emojicompletiondelegate.cpp
    common/authenticationcombobox.cpp
    common/flowlayout.cpp
    common/commandcompletiondelegate.cpp
    common/delegatepaintutil.cpp
    common/delegateutil.cpp
    )

set(Ruqola_textplugin_SRCS
    room/textpluginmanager.cpp
    room/plugins/plugintext.cpp
    room/plugins/plugintextinterface.cpp
    )

set(Ruqola_myaccount_SRCS
    myaccount/myaccountconfiguredialog.cpp
    myaccount/myaccountconfigurewidget.cpp
    myaccount/myaccount2faconfigurewidget.cpp
    myaccount/myaccountprofileconfigurewidget.cpp
    myaccount/myaccountpreferenceconfigurewidget.cpp
    myaccount/myaccountprofileconfigureavatarwidget.cpp
    )    

set(Ruqola_administrator_SRCS
    administratordialog/administratordialog.cpp
    administratordialog/administratorwidget.cpp
    administratordialog/administratorroomswidget.cpp
    administratordialog/administratorserverinfowidget.cpp
    administratordialog/administratorroomsselectroomtypewidget.cpp
    administratordialog/customuserstatus/administratorcustomuserstatuswidget.cpp
    administratordialog/customuserstatus/administratorcustomuserstatuscreatedialog.cpp
    administratordialog/customuserstatus/administratorcustomuserstatuscreatewidget.cpp
    )

ecm_qt_declare_logging_category(Ruqola_widgets_SRCS HEADER ruqolawidgets_debug.h IDENTIFIER RUQOLAWIDGETS_LOG CATEGORY_NAME org.kde.ruqola.widgets
    DESCRIPTION "ruqola widgets" EXPORT RUQOLA)
ecm_qt_declare_logging_category(Ruqola_widgets_SRCS HEADER ruqolawidgets_selection_debug.h IDENTIFIER RUQOLAWIDGETS_SELECTION_LOG CATEGORY_NAME org.kde.ruqola.widgets.selection
    DESCRIPTION "ruqola widgets (selection)" EXPORT RUQOLA)

qt5_add_resources(Ruqola_widgets_SRCS ruqolawidget.qrc)

add_library(libruqolawidgets
    ${Ruqola_widget_register_user_SRCS}
    ${Ruqola_textplugin_SRCS}
    ${ruqola_userfeedback_SRCS}
    ${Ruqola_common_SRCS}
    ${Ruqola_widgets_SRCS}
    ${Ruqola_widgets_dialog_SRCS}
    ${Ruqola_channellist_SRCS}
    ${Ruqola_roomwidget_SRCS}
    ${Ruqola_configure_SRCS}
    ${Ruqola_misc_widget_SRCS}
    ${Ruqola_threadwidget_SRCS}
    ${Ruqola_myaccount_SRCS}
    ${Ruqola_prunemessages_SRCS}
    ${Ruqola_administrator_SRCS}
    )

generate_export_header(libruqolawidgets BASE_NAME libruqolawidgets)

set(ruqola_userfeedback_LIB)
if (TARGET KUserFeedbackWidgets)
        set(ruqola_userfeedback_LIB KUserFeedbackWidgets)
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
    librocketchatrestapi-qt5
    libruqolacore
    ${ruqola_userfeedback_LIB}
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
endif()

install(TARGETS libruqolawidgets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
