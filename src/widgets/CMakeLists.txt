set(ruqola_userfeedback_SRCS)
if (TARGET KUserFeedbackWidgets)
    set(ruqola_userfeedback_SRCS ${ruqola_userfeedback_SRCS}
        userfeedback/userfeedbackmanager.cpp
        )
endif()


set(Ruqola_widgets_SRCS
    ruqolamainwindow.cpp
    ruqolacentralwidget.cpp
    ruqolaloginwidget.cpp
    ruqolamainwidget.cpp
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
    dialogs/showthreadswidget.cpp
    dialogs/showthreadsdialog.cpp
    dialogs/thread/listthreadsdelegate.cpp
    dialogs/searchchannel/searchchanneldelegate.cpp
    dialogs/takevideomessagedialog.cpp
    dialogs/takevideomessagewidget.cpp
    )

set(Ruqola_configure_SRCS
    configuredialog/configuresettingsdialog.cpp
    configuredialog/configureaccountwidget.cpp
    configuredialog/accountserverlistwidget.cpp
    configuredialog/configureaccountserverwidget.cpp
    configuredialog/configurespellcheckingwidget.cpp
    configuredialog/configureuserfeedbackwidget.cpp
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

set(Ruqola_roomwidget_SRCS
    room/roomwidget.cpp
    room/roomheaderwidget.cpp
    room/messagelistview.cpp
    room/messagetextedit.cpp
    room/delegate/messagelistdelegate.cpp
    room/delegate/messagedelegatehelperbase.cpp
    room/delegate/messagedelegatehelpertext.cpp
    room/delegate/messagedelegatehelperimage.cpp
    room/delegate/messagedelegatehelperfile.cpp
    room/delegate/messagedelegatehelperreactions.cpp
    room/messagelinewidget.cpp
    room/readonlylineeditwidget.cpp
    room/usersinroomflowwidget.cpp
    room/usersinroomlabel.cpp
    )

set(Ruqola_misc_widget_SRCS
    misc/accountmenu.cpp
    misc/accountsoverviewwidget.cpp
    misc/emoticonmenuwidget.cpp
    misc/emoticonselectorwidget.cpp
    misc/adduserswidget.cpp
    misc/clickableuserwidget.cpp
    )

set(Ruqola_common_SRCS
    common/completionlineedit.cpp
    common/completiontextedit.cpp
    common/completionlistview.cpp
    common/authenticationcombobox.cpp
    common/flowlayout.cpp
    )

ecm_qt_declare_logging_category(Ruqola_widgets_SRCS HEADER ruqolawidgets_debug.h IDENTIFIER RUQOLAWIDGETS_LOG CATEGORY_NAME org.kde.ruqola.widgets)

qt5_add_resources(Ruqola_widgets_SRCS ruqolawidget.qrc)

add_library(libruqolawidgets
    ${ruqola_userfeedback_SRCS}
    ${Ruqola_common_SRCS}
    ${Ruqola_widgets_SRCS}
    ${Ruqola_widgets_dialog_SRCS}
    ${Ruqola_channellist_SRCS}
    ${Ruqola_roomwidget_SRCS}
    ${Ruqola_configure_SRCS}
    ${Ruqola_misc_widget_SRCS}
    )

generate_export_header(libruqolawidgets BASE_NAME libruqolawidgets)

set(ruqola_userfeedback_LIB)
if (TARGET KUserFeedbackWidgets)
        set(ruqola_userfeedback_LIB KUserFeedbackWidgets)
endif()


target_link_libraries(libruqolawidgets
    Qt5::Gui
    Qt5::Widgets
    KF5::I18n
    KF5::ConfigCore
    KF5::XmlGui
    KF5::KIOWidgets
    KF5::WidgetsAddons
    KF5::SonnetUi
    KF5::TextWidgets
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
endif()

install(TARGETS libruqolawidgets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
