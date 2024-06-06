/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolamainwindow.h"
#include "databasedialog/exploredatabasedialog.h"
#include "explorepermissionsdialog/explorepermissionsdialog.h"
#include "misc/changefontsizemenu.h"
#include "notificationhistorymanager.h"
#include "rocketchaturlutils.h"
#include "ruqolaglobalconfig.h"
#include "ruqolawidgets_debug.h"

#include "accountmanager.h"
#include "administratordialog/administratordialog.h"
#include "administratorsettingsdialog/administratorsettingsdialog.h"
#include "servererrorinfohistory/servererrorinfomessagehistorydialog.h"

#include "bannerinfodialog/bannerinfodialog.h"
#include "configuredialog/configuresettingsdialog.h"
#include "configurenewserver/createnewserverdialog.h"
#include "connection.h"
#include "dialogs/channelpassworddialog.h"
#include "dialogs/createdirectmessagesdialog.h"
#include "dialogs/createnewchanneldialog.h"
#include "dialogs/createnewdiscussiondialog.h"
#include "dialogs/modifystatusdialog.h"
#include "dialogs/serverinfo/serverinfodialog.h"
#include "directory/directorydialog.h"
#include "importexportdata/exportdata/exportdatawizard.h"
#include "importexportdata/importdata/importdatawizard.h"
#include "localdatabase/localmessagelogger.h"
#include "misc/accountsoverviewwidget.h"
#include "misc/messagestylelayoutmenu.h"
#include "misc/servermenu.h"
#include "misc/statuscombobox.h"
#include "model/statusmodel.h"
#include "model/statusmodelfilterproxymodel.h"
#include "myaccount/myaccountconfiguredialog.h"
#include "notificationhistory/notificationhistorydialog.h"
#include "notifications/notification.h"
#include "ownuser/ownuserpreferences.h"
#include "receivetypingnotificationmanager.h"
#include "registeruser/registeruserdialog.h"
#include "rocketchataccount.h"
#include "room.h"
#include "ruqola.h"
#include "ruqolacentralwidget.h"
#include "ruqolacommandlineoptions.h"
#include "ruqolaserverconfig.h"
#include "switchchannelhistory/switchchanneltreeviewmanager.h"
#include "teams/teamscreatejob.h"
#include "whatsnew/whatsnewdialog.h"

#include <KActionCollection>
#include <KColorSchemeManager>
#include <KConfigGroup>
#include <KIO/JobUiDelegateFactory>
#include <KIO/OpenUrlJob>
#include <KLocalizedString>
#include <KMessageBox>
#include <KNotifyConfigWidget>
#include <KSharedConfig>
#include <KStandardAction>
#include <KToggleFullScreenAction>
#include <KToolBar>
#include <QActionGroup>
#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QIcon>
#include <QKeyCombination>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTemporaryFile>
#include <QToolButton>
#include <QWidgetAction>

#include <KColorSchemeMenu>

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
#include <KWindowSystem>
#endif

#if HAVE_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/NotificationPopup>
#include <KUserFeedback/Provider>
#endif
#include "colorsandmessageviewstyle.h"

namespace
{
static const char myRuqolaMainWindowGroupName[] = "RuqolaMainWindow";
const int ruqolaVersion = 1;
}

RuqolaMainWindow::RuqolaMainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
    , mMainWidget(new RuqolaCentralWidget(this))
    , mStatusProxyModel(new StatusModelFilterProxyModel(this))
    , mSwitchChannelTreeManager(new SwitchChannelTreeViewManager(this))
{
    mMainWidget->setObjectName(QStringLiteral("mMainWidget"));
    connect(mMainWidget, &RuqolaCentralWidget::loginPageActivated, this, &RuqolaMainWindow::slotLoginPageActivated);
    setCentralWidget(mMainWidget);
    setupActions();
    setupStatusBar();
    setupGUI(/*QStringLiteral(":/kxmlgui5/ruqola/ruqolaui.rc")*/);
    readConfig();
    createSystemTray();
    mSwitchChannelTreeManager->setParentWidget(mMainWidget);
    connect(mSwitchChannelTreeManager, &SwitchChannelTreeViewManager::switchToChannel, this, &RuqolaMainWindow::slotHistorySwitchChannel);
    mAccountManager = Ruqola::self()->accountManager();
    connect(mAccountManager, &AccountManager::currentAccountChanged, this, &RuqolaMainWindow::slotAccountChanged);
    connect(mAccountManager, &AccountManager::updateNotification, this, &RuqolaMainWindow::updateNotification);
    connect(mAccountManager, &AccountManager::roomNeedAttention, this, &RuqolaMainWindow::slotRoomNeedAttention);
    connect(mAccountManager, &AccountManager::logoutAccountDone, this, &RuqolaMainWindow::logout);

    slotAccountChanged();
#if HAVE_KUSERFEEDBACK
    auto userFeedBackNotificationPopup = new KUserFeedback::NotificationPopup(this);
    userFeedBackNotificationPopup->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
#endif
    mShowMenuBarAction->setChecked(RuqolaGlobalConfig::self()->showMenuBar());
    slotToggleMenubar(true);
}

RuqolaMainWindow::~RuqolaMainWindow()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group = config->group(QLatin1StringView(myRuqolaMainWindowGroupName));
    group.writeEntry("Size", size());

    delete mMainWidget; // before Ruqola::destroy()

    Ruqola::destroy();
}

void RuqolaMainWindow::parseCommandLine(QCommandLineParser *parser)
{
    if (parser->isSet(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::MessageUrl))) {
        const QString messageUrl = parser->value(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::MessageUrl));
        if (!messageUrl.isEmpty()) {
            if (RocketChatUrlUtils::parseUrl(messageUrl)) {
                return;
            }
        }
    }
    if (parser->isSet(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::Account))) {
        const QString loadAccount = parser->value(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::Account));
        if (!loadAccount.isEmpty()) {
            Ruqola::self()->setCurrentAccount(loadAccount);
        }
    }
}

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
void RuqolaMainWindow::slotActivateRequested(const QStringList &arguments, const QString &workingDirectory)
{
    Q_UNUSED(workingDirectory)
    if (!arguments.isEmpty()) {
        QCommandLineParser parser;
        RuqolaCommandLineParser commandLineParser(&parser);
        parser.parse(arguments);
        parseCommandLine(&parser);
    }

    KWindowSystem::updateStartupId(windowHandle());
    KWindowSystem::activateWindow(windowHandle());
}
#endif

void RuqolaMainWindow::slotRoomNeedAttention()
{
    if (mNotification) {
        mNotification->roomNeedAttention();
    }
}

void RuqolaMainWindow::logout(const QString &accountName)
{
    if (mNotification) {
        mNotification->clearNotification(accountName);
    }
}

void RuqolaMainWindow::updateNotification(bool hasAlert, int nbUnread, const QString &accountName)
{
    if (mNotification) {
        mNotification->updateNotification(hasAlert, nbUnread, accountName);
    }
}

void RuqolaMainWindow::setupStatusBar()
{
    statusBar()->insertPermanentWidget(0, mContainerStatusInfo);
    mStatusBarTypingMessage = new QLabel(this);
    mStatusBarTypingMessage->setTextFormat(Qt::RichText);
    mStatusBarTypingMessage->setObjectName(QStringLiteral("mStatusBarTypingMessage"));
    statusBar()->addPermanentWidget(mStatusBarTypingMessage, 1);
    mAccountOverviewWidget = new AccountsOverviewWidget(this);
    mAccountOverviewWidget->setObjectName(QStringLiteral("mAccountOverviewWidget"));
    statusBar()->addPermanentWidget(mAccountOverviewWidget);
    mNotificationToolButton = new QToolButton(this);
    mNotificationToolButton->setIcon(QIcon::fromTheme(QStringLiteral("notifications")));
    mNotificationToolButton->setObjectName(QStringLiteral("mNotificationToolButton"));
    mNotificationToolButton->setToolTip(i18nc("@info:tooltip", "Show New Notifications"));
#ifndef QT_NO_ACCESSIBILITY
    mNotificationToolButton->setAccessibleName(i18n("Show New Notifications"));
#endif

    mNotificationToolButton->hide(); // Hide at start
    mNotificationToolButton->setAutoRaise(true);
    connect(mNotificationToolButton, &QToolButton::clicked, this, &RuqolaMainWindow::slotOpenNotificationHistory);
    statusBar()->addPermanentWidget(mNotificationToolButton);
    connect(NotificationHistoryManager::self(), &NotificationHistoryManager::newNotification, this, &RuqolaMainWindow::slotNewNotification);
}

void RuqolaMainWindow::slotNewNotification()
{
    mNotificationToolButton->show();
}

void RuqolaMainWindow::slotAccountChanged()
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, nullptr, this, nullptr);
        disconnect(mCurrentRocketChatAccount->receiveTypingNotificationManager(), nullptr, this, nullptr);
    }
    mCurrentRocketChatAccount = Ruqola::self()->rocketChatAccount();
    connect(mCurrentRocketChatAccount->receiveTypingNotificationManager(),
            &ReceiveTypingNotificationManager::notificationChanged,
            this,
            &RuqolaMainWindow::slotTypingNotificationChanged);
    connect(mCurrentRocketChatAccount->receiveTypingNotificationManager(),
            &ReceiveTypingNotificationManager::clearNotification,
            this,
            &RuqolaMainWindow::slotClearNotification);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::missingChannelPassword, this, &RuqolaMainWindow::slotMissingChannelPassword);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::publicSettingChanged, this, &RuqolaMainWindow::updateActions);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::serverVersionChanged, this, [this]() {
        slotPermissionChanged();
        updateActions();
    });
    connect(mCurrentRocketChatAccount, &RocketChatAccount::ownInfoChanged, this, [this]() {
        updateActions();
        slotPermissionChanged();
    });
    connect(mCurrentRocketChatAccount, &RocketChatAccount::ownUserUiPreferencesChanged, this, [this]() {
        updateActions();
    });
    connect(mCurrentRocketChatAccount, &RocketChatAccount::raiseWindow, this, &RuqolaMainWindow::slotRaiseWindow);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::permissionChanged, this, &RuqolaMainWindow::slotPermissionChanged);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::registerUserSuccess, this, &RuqolaMainWindow::slotRegisterUserSuccessed);
    connect(mCurrentRocketChatAccount,
            &RocketChatAccount::userStatusUpdated,
            this,
            [this](User::PresenceStatus status, const QString &customText, const QString &accountName) {
                if (mCurrentRocketChatAccount->accountName() == accountName) {
                    mStatusComboBox->blockSignals(true);
                    mStatusComboBox->setStatus(status, customText);
                    mStatusComboBox->blockSignals(false);
                }
            });
    connect(mCurrentRocketChatAccount, &RocketChatAccount::customStatusChanged, this, &RuqolaMainWindow::slotUpdateCustomUserStatus);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::privateSettingsChanged, this, &RuqolaMainWindow::slotPrivateSettingsChanged);
    connect(mCurrentRocketChatAccount, &RocketChatAccount::publicSettingChanged, this, &RuqolaMainWindow::slotPrivateSettingsChanged);

    updateActions();
    slotClearNotification(); // Clear notification when we switch too.
    mMainWidget->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    mSwitchChannelTreeManager->setCurrentRocketChatAccount(mCurrentRocketChatAccount);

    mStatusComboBox->blockSignals(true);
    mStatusProxyModel->setSourceModel(mCurrentRocketChatAccount->statusModel());
    slotPrivateSettingsChanged();
    mStatusComboBox->setModel(mStatusProxyModel);

    slotUpdateCustomUserStatus();
    mStatusComboBox->setStatus(mCurrentRocketChatAccount->presenceStatus());
    mStatusComboBox->blockSignals(false);

    slotUpdateStatusMenu();
}

void RuqolaMainWindow::slotPrivateSettingsChanged()
{
    mStatusProxyModel->setAllowOfflineSupport(mCurrentRocketChatAccount->ruqolaServerConfig()->accountsAllowInvisibleStatusOption());
    mStatusProxyModel->setUseOnlyStandardStatus(!mCurrentRocketChatAccount->ruqolaServerConfig()->allowCustomStatusMessage());
}

void RuqolaMainWindow::slotRaiseWindow()
{
    show();
    raise();
#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
    KWindowSystem::activateWindow(windowHandle());
#else
    activateWindow();
#endif
}

void RuqolaMainWindow::slotPermissionChanged()
{
    mCreateNewChannel->setEnabled(canCreateChannels());
    mCreateDirectMessages->setEnabled(canCreateDirectMessages());
    mCreateTeam->setEnabled(canCreateTeams());
}

void RuqolaMainWindow::updateActions()
{
    mUnreadOnTop->setChecked(mCurrentRocketChatAccount->ownUserPreferences().showUnread());
    const auto roomListSortOrder = mCurrentRocketChatAccount->ownUserPreferences().roomListSortOrder();
    mRoomListSortByLastMessage->setChecked(roomListSortOrder == OwnUserPreferences::RoomListSortOrder::ByLastMessage);
    mRoomListSortAlphabetically->setChecked(roomListSortOrder == OwnUserPreferences::RoomListSortOrder::Alphabetically);

    const auto roomListDisplay = mCurrentRocketChatAccount->ownUserPreferences().roomListDisplay();
    mRoomListDisplayMedium->setChecked(roomListDisplay == OwnUserPreferences::RoomListDisplay::Medium);
    mRoomListDisplayCondensed->setChecked(roomListDisplay == OwnUserPreferences::RoomListDisplay::Condensed);
    mRoomListDisplayExtended->setChecked(roomListDisplay == OwnUserPreferences::RoomListDisplay::Extended);

    mRegisterNewUser->setVisible(mCurrentRocketChatAccount->registrationFormEnabled());
    mCreateDiscussion->setEnabled(mCurrentRocketChatAccount->discussionEnabled()
                                  && (mCurrentRocketChatAccount->loginStatus() == AuthenticationManager::LoggedIn));
    const bool isAdministrator{mCurrentRocketChatAccount->isAdministrator()};
    mAdministrator->setEnabled(isAdministrator);
    mAdministratorServerSettings->setEnabled(isAdministrator);
    mAdministrationMenu->setVisible(isAdministrator);
    mShowRocketChatServerInfo->setVisible(hasBannerInfo());
    mRoomAvatar->setChecked(mCurrentRocketChatAccount->ownUserPreferences().showRoomAvatar());
    mRoomFavorite->setChecked(mCurrentRocketChatAccount->ownUserPreferences().showFavorite());
    mCreateNewChannel->setEnabled(canCreateChannels());
    mCreateDirectMessages->setEnabled(canCreateDirectMessages());
    mCreateTeam->setEnabled(canCreateTeams());
}

bool RuqolaMainWindow::canCreateChannels() const
{
    return mCurrentRocketChatAccount && mCurrentRocketChatAccount->hasPermission(QStringLiteral("create-c"));
}

bool RuqolaMainWindow::canCreateDirectMessages() const
{
    return mCurrentRocketChatAccount && mCurrentRocketChatAccount->hasPermission(QStringLiteral("create-d"));
}

bool RuqolaMainWindow::canCreateTeams() const
{
    return mCurrentRocketChatAccount && mCurrentRocketChatAccount->hasPermission(QStringLiteral("create-team"));
}

bool RuqolaMainWindow::hasBannerInfo() const
{
    return mCurrentRocketChatAccount && !mCurrentRocketChatAccount->bannerInfos().isEmpty();
}

void RuqolaMainWindow::readConfig()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group = KConfigGroup(config, QLatin1StringView(myRuqolaMainWindowGroupName));
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void RuqolaMainWindow::slotClearNotification()
{
    mStatusBarTypingMessage->clear();
}

void RuqolaMainWindow::slotTypingNotificationChanged(const QByteArray &roomId, const QString &notificationStr)
{
    if (mMainWidget->roomId() == roomId) {
        mStatusBarTypingMessage->setText(notificationStr);
    }
}

void RuqolaMainWindow::setupActions()
{
    KActionCollection *ac = actionCollection();

    mShowMenuBarAction = KStandardAction::showMenubar(this, &RuqolaMainWindow::slotToggleMenubar, ac);

    KStandardAction::quit(this, &RuqolaMainWindow::slotClose, ac);
    KStandardAction::preferences(this, &RuqolaMainWindow::slotConfigure, ac);
    KStandardAction::configureNotifications(this, &RuqolaMainWindow::slotConfigureNotifications, ac);

    auto act = new QAction(i18nc("@action", "Add Server…"), this);
    connect(act, &QAction::triggered, this, &RuqolaMainWindow::slotAddServer);
    ac->addAction(QStringLiteral("add_server"), act);

    // Move in specific server widget
    mServerInfo = new QAction(i18nc("@action", "Server Info…"), this);
    connect(mServerInfo, &QAction::triggered, this, &RuqolaMainWindow::slotServerInfo);
    ac->addAction(QStringLiteral("server_info"), mServerInfo);

    mLogout = new QAction(i18nc("@action", "Logout Current Server"), this);
    connect(mLogout, &QAction::triggered, this, &RuqolaMainWindow::slotLogout);
    ac->addAction(QStringLiteral("logout"), mLogout);

    mNextUnreadChannel = new QAction(i18nc("@action", "Jump to Next Unread Channel"), this);
    connect(mNextUnreadChannel, &QAction::triggered, this, &RuqolaMainWindow::slotSelectNextUnreadChannel);
    ac->setDefaultShortcut(mNextUnreadChannel, Qt::Key_Control | Qt::Key_PageDown);
    ac->addAction(QStringLiteral("next_unread_channel"), mNextUnreadChannel);

    mCreateNewChannel = new QAction(QIcon::fromTheme(QStringLiteral("irc-join-channel")), i18n("Create New Channel…"), this);
    connect(mCreateNewChannel, &QAction::triggered, this, &RuqolaMainWindow::slotCreateNewChannel);
    ac->addAction(QStringLiteral("create_new_channel"), mCreateNewChannel);

    mCreateDirectMessages = new QAction(QIcon::fromTheme(QStringLiteral("irc-join-channel")), i18n("Create Direct Messages…"), this);
    connect(mCreateDirectMessages, &QAction::triggered, this, &RuqolaMainWindow::slotCreateDirectMessages);
    ac->addAction(QStringLiteral("create_direct_messages"), mCreateDirectMessages);

    mCreateDiscussion = new QAction(QIcon::fromTheme(QStringLiteral("irc-join-channel")), i18n("Create Discussion…"), this);
    connect(mCreateDiscussion, &QAction::triggered, this, &RuqolaMainWindow::slotCreateDiscussion);
    ac->addAction(QStringLiteral("create_discussion"), mCreateDiscussion);

    mCreateTeam = new QAction(QIcon::fromTheme(QStringLiteral("irc-join-channel")), i18n("Create Team…"), this);
    connect(mCreateTeam, &QAction::triggered, this, &RuqolaMainWindow::slotCreateTeam);
    ac->addAction(QStringLiteral("create_team"), mCreateTeam);

    mServerMenu = new ServerMenu(this);
    mServerMenu->setActionCollection(ac);
    ac->addAction(QStringLiteral("server_menu"), mServerMenu);

    mUnreadOnTop = new QAction(i18nc("@action", "Unread on Top"), this);
    mUnreadOnTop->setCheckable(true);
    connect(mUnreadOnTop, &QAction::triggered, this, &RuqolaMainWindow::slotUnreadOnTop);
    ac->addAction(QStringLiteral("unread_on_top"), mUnreadOnTop);

    auto roomListSortOrder = new QActionGroup(this);
    roomListSortOrder->setExclusive(true);

    mRoomListSortByLastMessage = new QAction(i18nc("@action", "By Last Message"), this);
    mRoomListSortByLastMessage->setCheckable(true);
    connect(mRoomListSortByLastMessage, &QAction::triggered, this, &RuqolaMainWindow::slotRoomListSortByLastMessage);
    roomListSortOrder->addAction(mRoomListSortByLastMessage);
    ac->addAction(QStringLiteral("room_list_sort_by_last_message"), mRoomListSortByLastMessage);

    mRoomListSortAlphabetically = new QAction(i18nc("@action", "Alphabetically"), this);
    mRoomListSortAlphabetically->setCheckable(true);
    connect(mRoomListSortAlphabetically, &QAction::triggered, this, &RuqolaMainWindow::slotRoomListSortAlphabetically);
    roomListSortOrder->addAction(mRoomListSortAlphabetically);
    ac->addAction(QStringLiteral("room_list_sort_alphabetically"), mRoomListSortAlphabetically);

    mShowLog = new QAction(QIcon::fromTheme(QStringLiteral("view-history")), i18n("Show Channel Log"), this);
    connect(mShowLog, &QAction::triggered, this, &RuqolaMainWindow::slotShowLog);
    ac->addAction(QStringLiteral("show_log"), mShowLog);

    mShowServerInfo = new QAction(QIcon::fromTheme(QStringLiteral("data-error")), i18n("Show Server Errors"), this);
    connect(mShowServerInfo, &QAction::triggered, this, &RuqolaMainWindow::slotShowServerInfo);
    ac->addAction(QStringLiteral("show_server_errors"), mShowServerInfo);

    if (Ruqola::self()->debug()) {
        mMenuDebug = new QAction(QStringLiteral("Debug"), this);
        ac->addAction(QStringLiteral("debug_menu"), mMenuDebug);
        auto menu = new QMenu(this);
        mMenuDebug->setMenu(menu);
        mShowDatabaseMessages = new QAction(QStringLiteral("Show Database Messages"), this);
        connect(mShowDatabaseMessages, &QAction::triggered, this, &RuqolaMainWindow::slotShowDatabaseMessages);
        ac->addAction(QStringLiteral("show_database_messages"), mShowDatabaseMessages);
        menu->addAction(mShowDatabaseMessages);
        mShowPermissions = new QAction(QStringLiteral("Show Permissions"), this);
        connect(mShowPermissions, &QAction::triggered, this, &RuqolaMainWindow::slotShowPermissions);
        ac->addAction(QStringLiteral("show_permissions"), mShowPermissions);
        menu->addAction(mShowPermissions);
    }

    mClearAlerts = new QAction(i18nc("@action", "Mark All Channels as Read"), this);
    ac->setDefaultShortcut(mClearAlerts, Qt::SHIFT | Qt::Key_Escape);
    connect(mClearAlerts, &QAction::triggered, this, &RuqolaMainWindow::slotClearAccountAlerts);
    ac->addAction(QStringLiteral("mark_all_channels_read"), mClearAlerts);

    mRegisterNewUser = new QAction(i18nc("@action", "Register a New User…"), this);
    connect(mRegisterNewUser, &QAction::triggered, this, &RuqolaMainWindow::slotRegisterNewUser);
    ac->addAction(QStringLiteral("register_new_user"), mRegisterNewUser);

    mMyAccount = new QAction(i18nc("@action", "My Account…"), this);
    connect(mMyAccount, &QAction::triggered, this, &RuqolaMainWindow::slotConfigureMyAccount);
    ac->addAction(QStringLiteral("configure_my_account"), mMyAccount);

    mAdministrationMenu = new KActionMenu(i18n("administration"), this);
    mAdministrator = new QAction(i18nc("@action", "Administrator…"), this);
    ac->addAction(QStringLiteral("administrator_account_settings"), mAdministrator);
    connect(mAdministrator, &QAction::triggered, this, &RuqolaMainWindow::slotAdministrator);
    mAdministrationMenu->addAction(mAdministrator);
    ac->addAction(QStringLiteral("administration"), mAdministrationMenu);

    mAdministratorServerSettings = new QAction(i18nc("@action", "Server Settings…"), this);
    connect(mAdministratorServerSettings, &QAction::triggered, this, &RuqolaMainWindow::slotAdministratorServerSettings);
    ac->addAction(QStringLiteral("administrator_server_settings"), mAdministratorServerSettings);
    mAdministrationMenu->addAction(mAdministratorServerSettings);

    mAdministrationMenu->addSeparator();

    mApplications = new QAction(i18nc("@action", "Applications…"), this);
    connect(mApplications, &QAction::triggered, this, &RuqolaMainWindow::slotApplicationsSettings);
    ac->addAction(QStringLiteral("administrator_server_settings"), mApplications);
    mAdministrationMenu->addAction(mApplications);

    mDirectory = new QAction(i18nc("Action which allows to search room/user/team", "Directory…"), this);
    connect(mDirectory, &QAction::triggered, this, &RuqolaMainWindow::slotDirectory);
    ac->addAction(QStringLiteral("directory"), mDirectory);

    // Actions to navigate through the different pages
    QList<QKeySequence> nextShortcut;
    QList<QKeySequence> prevShortcut;

    QString nextIcon, prevIcon;
    if (QApplication::isRightToLeft()) {
        prevShortcut.append(QKeySequence(QStringLiteral("Alt+Right")));
        nextShortcut.append(QKeySequence(QStringLiteral("Alt+Left")));
        nextIcon = QStringLiteral("go-previous-view");
        prevIcon = QStringLiteral("go-next-view");
    } else {
        nextShortcut.append(QKeySequence(QStringLiteral("Alt+Right")));
        prevShortcut.append(QKeySequence(QStringLiteral("Alt+Left")));
        nextIcon = QStringLiteral("go-next-view");
        prevIcon = QStringLiteral("go-previous-view");
    }

    mNextTab = new QAction(QIcon::fromTheme(nextIcon), i18n("&Next Server"), this);
    actionCollection()->setDefaultShortcuts(mNextTab, nextShortcut);
    connect(mNextTab, &QAction::triggered, this, &RuqolaMainWindow::showNextView);
    actionCollection()->addAction(QStringLiteral("next_tab"), mNextTab);

    mPreviewTab = new QAction(QIcon::fromTheme(prevIcon), i18n("&Previous Server"), this);
    actionCollection()->setDefaultShortcuts(mPreviewTab, prevShortcut);
    connect(mPreviewTab, &QAction::triggered, this, &RuqolaMainWindow::showPreviousView);
    actionCollection()->addAction(QStringLiteral("previous_tab"), mPreviewTab);

    {
        mContainerStatusInfo = new QWidget(this);
        auto layout = new QHBoxLayout(mContainerStatusInfo);
        layout->setContentsMargins({});
        auto label = new QLabel(i18nc("@label:textbox", "Status:"), mContainerStatusInfo);
        label->setObjectName(QStringLiteral("label"));
        layout->addWidget(label);

        mStatusComboBox = new StatusCombobox(mContainerStatusInfo);
        mStatusComboBox->setObjectName(QStringLiteral("mStatusComboBox"));
        layout->addWidget(mStatusComboBox);
        connect(mStatusComboBox, &StatusCombobox::currentIndexChanged, this, &RuqolaMainWindow::slotStatusChanged);
        connect(mStatusComboBox, &StatusCombobox::currentIndexChanged, this, &RuqolaMainWindow::slotUpdateStatusMenu);
    }

    {
        QList<QAction *> listActions;
        auto act = new QAction(i18nc("@action", "Previous Selected Channel"), this);
        ac->setDefaultShortcut(act, QKeySequence(Qt::CTRL | Qt::Key_Tab));
        ac->addAction(QStringLiteral("previous_channel"), act);
        listActions.append(act);

        connect(act, &QAction::triggered, this, &RuqolaMainWindow::undoSwitchChannel);

        act = new QAction(i18nc("@action", "Next Selected Channel"), this);
        ac->addAction(QStringLiteral("next_channel"), act);
        QKeyCombination combinationKeys(Qt::CTRL | Qt::SHIFT, Qt::Key_Tab);
        ac->setDefaultShortcut(act, combinationKeys);
        connect(act, &QAction::triggered, this, &RuqolaMainWindow::redoSwitchChannel);
        listActions.append(act);

        mSwitchChannelTreeManager->addActions(listActions);
    }

    if (menuBar()) {
        mHamburgerMenu = KStandardAction::hamburgerMenu(nullptr, nullptr, actionCollection());
        mHamburgerMenu->setShowMenuBarAction(mShowMenuBarAction);
        mHamburgerMenu->setMenuBar(menuBar());
        mHamburgerMenu->hideActionsOf(toolBar());
        connect(mHamburgerMenu, &KHamburgerMenu::aboutToShowMenu, this, [this]() {
            updateHamburgerMenu();
            // Immediately disconnect. We only need to run this once, but on demand.
            // NOTE: The nullptr at the end disconnects all connections between
            // q and mHamburgerMenu's aboutToShowMenu signal.
            disconnect(mHamburgerMenu, &KHamburgerMenu::aboutToShowMenu, this, nullptr);
        });
    }

    auto manager = new KColorSchemeManager(this);
    ac->addAction(QStringLiteral("colorscheme_menu"), KColorSchemeMenu::createMenu(manager, this));

    mShowFullScreenAction = KStandardAction::fullScreen(nullptr, nullptr, this, ac);
    ac->setDefaultShortcut(mShowFullScreenAction, Qt::Key_F11);
    connect(mShowFullScreenAction, &QAction::toggled, this, &RuqolaMainWindow::slotFullScreen);

    mShowNotifyHistory = new QAction(QIcon::fromTheme(QStringLiteral("notifications")), i18n("Show Notify History"), this);
    connect(mShowNotifyHistory, &QAction::triggered, this, &RuqolaMainWindow::slotOpenNotificationHistory);
    ac->addAction(QStringLiteral("show_notify_history"), mShowNotifyHistory);

    mShowRocketChatServerInfo = new QAction(i18nc("@action", "Show RocketChat Information"), this);
    connect(mShowRocketChatServerInfo, &QAction::triggered, this, &RuqolaMainWindow::slotRocketChatInformation);
    ac->addAction(QStringLiteral("show_rocketchat_information"), mShowRocketChatServerInfo);

    mRoomAvatar = new QAction(i18nc("@action", "Show Room Avatar"), this);
    mRoomAvatar->setCheckable(true);
    connect(mRoomAvatar, &QAction::triggered, this, &RuqolaMainWindow::slotShowRoomAvatar);
    ac->addAction(QStringLiteral("room_avatar"), mRoomAvatar);

    mRoomFavorite = new QAction(i18nc("@action", "Show Favorite Room"), this);
    mRoomFavorite->setCheckable(true);
    connect(mRoomFavorite, &QAction::triggered, this, &RuqolaMainWindow::slotShowFavoriteRoom);
    ac->addAction(QStringLiteral("room_favorite"), mRoomFavorite);

    auto showWhatsNewAction = new QAction(QIcon::fromTheme(QStringLiteral("ruqola")), i18n("What's new"), this);
    ac->addAction(QStringLiteral("whatsnew_ruqola"), showWhatsNewAction);
    connect(showWhatsNewAction, &QAction::triggered, this, &RuqolaMainWindow::slotWhatsNew);

    mImportAccountsAction = new QAction(QIcon::fromTheme(QStringLiteral("document-import")), i18n("Import Accounts"), this);
    connect(mImportAccountsAction, &QAction::triggered, this, &RuqolaMainWindow::slotImportAccounts);
    ac->addAction(QStringLiteral("import_accounts"), mImportAccountsAction);

    mExportAccountsAction = new QAction(QIcon::fromTheme(QStringLiteral("document-export")), i18n("Export Accounts"), this);
    connect(mExportAccountsAction, &QAction::triggered, this, &RuqolaMainWindow::slotExportAccounts);
    ac->addAction(QStringLiteral("export_accounts"), mExportAccountsAction);

    auto messageStyleAction = new MessageStyleLayoutMenu(this);
    ac->addAction(QStringLiteral("message_style"), messageStyleAction);
    connect(messageStyleAction, &MessageStyleLayoutMenu::styleChanged, this, &RuqolaMainWindow::slotMessageStyleChanged);

    auto changeFontSizeAction = new ChangeFontSizeMenu(this);
    ac->addAction(QStringLiteral("change_font_size"), changeFontSizeAction);
    connect(changeFontSizeAction, &ChangeFontSizeMenu::fontChanged, this, [] {
        Q_EMIT ColorsAndMessageViewStyle::self().needUpdateFontSize();
    });

    auto roomListDisplay = new QActionGroup(this);
    roomListDisplay->setExclusive(true);

    mRoomListDisplayMedium = new QAction(i18nc("@action", "Medium"), this);
    mRoomListDisplayMedium->setCheckable(true);
    connect(mRoomListDisplayMedium, &QAction::triggered, this, [this]() {
        mCurrentRocketChatAccount->setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Medium);
    });
    roomListDisplay->addAction(mRoomListDisplayMedium);
    ac->addAction(QStringLiteral("room_list_display_medium"), mRoomListDisplayMedium);

    mRoomListDisplayCondensed = new QAction(i18nc("@action", "Condensed"), this);
    mRoomListDisplayCondensed->setCheckable(true);
    connect(mRoomListDisplayCondensed, &QAction::triggered, this, [this]() {
        mCurrentRocketChatAccount->setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Condensed);
    });

    roomListDisplay->addAction(mRoomListDisplayCondensed);
    ac->addAction(QStringLiteral("room_list_display_condensed"), mRoomListDisplayCondensed);

    mRoomListDisplayExtended = new QAction(i18nc("@action", "Extended"), this);
    mRoomListDisplayExtended->setCheckable(true);
    connect(mRoomListDisplayExtended, &QAction::triggered, this, [this]() {
        mCurrentRocketChatAccount->setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Extended);
    });
    roomListDisplay->addAction(mRoomListDisplayExtended);
    ac->addAction(QStringLiteral("room_list_display_extended"), mRoomListDisplayExtended);
}

void RuqolaMainWindow::slotMessageStyleChanged()
{
    Q_EMIT ColorsAndMessageViewStyle::self().needUpdateMessageStyle();
}

void RuqolaMainWindow::slotWhatsNew()
{
    WhatsNewDialog dlg(this);
    dlg.updateInformations();
    dlg.exec();
}

void RuqolaMainWindow::showNextView()
{
    mAccountOverviewWidget->showNextView();
}

void RuqolaMainWindow::showPreviousView()
{
    mAccountOverviewWidget->showPreviousView();
}

void RuqolaMainWindow::slotClearAccountAlerts()
{
    if (auto acct = mAccountManager->account()) {
        acct->clearAllUnreadMessages();
    }
}

void RuqolaMainWindow::slotCreateTeam()
{
    QPointer<CreateNewChannelDialog> dlg = new CreateNewChannelDialog(mCurrentRocketChatAccount, this);
    dlg->setWindowTitle(i18nc("@title:window", "Create Team"));
    CreateNewChannelWidget::Features flags;
    if (mCurrentRocketChatAccount->broadCastEnabled()) {
        flags |= CreateNewChannelWidget::Feature::BroadCast;
    }
    if (mCurrentRocketChatAccount->encryptionEnabled()) {
        flags |= CreateNewChannelWidget::Feature::Encrypted;
    }
    dlg->setFeatures(flags);
    if (dlg->exec()) {
        RocketChatRestApi::CreateChannelTeamInfo teamInfo = dlg->channelInfo(true);
        teamInfo.infoType = RocketChatRestApi::CreateChannelTeamInfo::CreateInfoType::Team;
        auto job = new RocketChatRestApi::TeamsCreateJob(this);
        job->setTeamsCreateJobInfo(teamInfo);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::TeamsCreateJob::teamCreateDone, this, []() {
            qCDebug(RUQOLAWIDGETS_LOG) << " teamCreateDone";
            // TODO switch to new team ?
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamsCreateJob";
        }
    }
    delete dlg;
}

void RuqolaMainWindow::slotCreateDiscussion()
{
    CreateNewDiscussionDialog dlg(mCurrentRocketChatAccount, this);
    dlg.exec();
}

void RuqolaMainWindow::slotCreateDirectMessages()
{
    CreateDirectMessagesDialog dlg(mCurrentRocketChatAccount, this);
    dlg.exec();
}

void RuqolaMainWindow::slotCreateNewChannel()
{
    QPointer<CreateNewChannelDialog> dlg = new CreateNewChannelDialog(mCurrentRocketChatAccount, this);
    CreateNewChannelWidget::Features flags;
    if (mCurrentRocketChatAccount->broadCastEnabled()) {
        flags |= CreateNewChannelWidget::Feature::BroadCast;
    }
    if (mCurrentRocketChatAccount->encryptionEnabled()) {
        flags |= CreateNewChannelWidget::Feature::Encrypted;
    }
    dlg->setFeatures(flags);
    if (dlg->exec()) {
        RocketChatRestApi::CreateChannelTeamInfo info = dlg->channelInfo(false);
        info.infoType = RocketChatRestApi::CreateChannelTeamInfo::CreateInfoType::Channel;
        mCurrentRocketChatAccount->createNewChannel(info);
    }
    delete dlg;
}

void RuqolaMainWindow::slotConfigure()
{
    QPointer<ConfigureSettingsDialog> dlg = new ConfigureSettingsDialog(this);
    if (dlg->exec()) {
        if (RuqolaGlobalConfig::self()->useCustomFont()) {
            qApp->setFont(RuqolaGlobalConfig::self()->generalFont());
        } else {
            qApp->setFont(QFontDatabase::systemFont(QFontDatabase::GeneralFont));
        }

        mAccountOverviewWidget->updateButtons();
        createSystemTray();
        Q_EMIT Ruqola::self()->translatorMenuChanged();
        Q_EMIT ColorsAndMessageViewStyle::self().needUpdateFontSize();
    }
    delete dlg;
}

void RuqolaMainWindow::slotAddServer()
{
    QPointer<CreateNewServerDialog> dlg = new CreateNewServerDialog(this);
    const QStringList lst = mAccountManager->accountsName();
    dlg->setExistingAccountName(lst);
    if (dlg->exec()) {
        const AccountManager::AccountManagerInfo info = dlg->accountInfo();
        mAccountManager->addAccount(std::move(info));
    }
    delete dlg;
}

void RuqolaMainWindow::slotServerInfo()
{
    ServerInfoDialog dlg(mCurrentRocketChatAccount, this);
    dlg.exec();
}

void RuqolaMainWindow::slotLogout()
{
    mCurrentRocketChatAccount->logOut();
}

void RuqolaMainWindow::slotSelectNextUnreadChannel()
{
    mMainWidget->selectNextUnreadChannel();
}

void RuqolaMainWindow::slotUnreadOnTop(bool checked)
{
    mCurrentRocketChatAccount->setSortUnreadOnTop(checked);
}

void RuqolaMainWindow::slotRoomListSortByLastMessage()
{
    mCurrentRocketChatAccount->setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::ByLastMessage);
}

void RuqolaMainWindow::slotRoomListSortAlphabetically()
{
    mCurrentRocketChatAccount->setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::Alphabetically);
}

void RuqolaMainWindow::slotShowDatabaseMessages()
{
    // Use only when we want to debug and has database support
    ExploreDatabaseDialog dlg(mCurrentRocketChatAccount, this);
    dlg.exec();
}

void RuqolaMainWindow::slotShowPermissions()
{
    ExplorePermissionsDialog dlg(this);
    dlg.setPermissions(mCurrentRocketChatAccount->permissions());
    dlg.setOWnRoles(mCurrentRocketChatAccount->ownUserPermission());
    dlg.exec();
}

void RuqolaMainWindow::slotShowServerInfo()
{
    ServerErrorInfoMessageHistoryDialog dlg(this);
    dlg.addServerList(Ruqola::self()->accountManager()->accountDisplayInfoSorted());
    dlg.exec();
}

void RuqolaMainWindow::slotShowLog()
{
    auto *room = mMainWidget->room();
    if (room) {
        QTemporaryFile tempFile(QDir::tempPath() + QStringLiteral("/XXXXXX.log.txt"));
        tempFile.setAutoRemove(false);
        if (tempFile.open()) {
            LocalMessageLogger logger;
            if (logger.saveToFile(tempFile, mCurrentRocketChatAccount->accountName(), room->displayFName())) {
                const QString fileName = tempFile.fileName();
                tempFile.close();
                auto job = new KIO::OpenUrlJob(QUrl::fromLocalFile(fileName), this);
                job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, this));
                job->setDeleteTemporaryFile(true);
                job->start();
            } else {
                KMessageBox::information(this, i18n("Impossible to open log."), i18n("Show Log"));
            }
        }
    }
}

void RuqolaMainWindow::slotMissingChannelPassword(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo)
{
    QPointer<ChannelPasswordDialog> dlg = new ChannelPasswordDialog(this);
    // TODO add channel name!
    if (dlg->exec()) {
        // FIXME channelinfo
        mCurrentRocketChatAccount->joinRoom(channelInfo.identifier, dlg->password());
    }
    delete dlg;
}

void RuqolaMainWindow::slotLoginPageActivated(bool loginPageActivated)
{
    mCreateNewChannel->setEnabled(!loginPageActivated && canCreateChannels());
    mCreateDirectMessages->setEnabled(!loginPageActivated && canCreateDirectMessages());
    mLogout->setEnabled(!loginPageActivated);
    mClearAlerts->setEnabled(!loginPageActivated);
    mMyAccount->setEnabled(!loginPageActivated);
    mContainerStatusInfo->setEnabled(!loginPageActivated);
    mCreateDiscussion->setEnabled(!loginPageActivated);
    mCreateTeam->setEnabled(!loginPageActivated && canCreateTeams());
    mDirectory->setEnabled(!loginPageActivated);
    mNextUnreadChannel->setEnabled(!loginPageActivated);
    mShowLog->setEnabled(!loginPageActivated);
    mShowRocketChatServerInfo->setVisible(!loginPageActivated && hasBannerInfo());
    mRoomAvatar->setEnabled(!loginPageActivated);
    mUnreadOnTop->setEnabled(!loginPageActivated);
    mRoomListSortByLastMessage->setEnabled(!loginPageActivated);
    mRoomListSortAlphabetically->setEnabled(!loginPageActivated);

    mRoomListDisplayMedium->setEnabled(!loginPageActivated);
    mRoomListDisplayCondensed->setEnabled(!loginPageActivated);
    mRoomListDisplayExtended->setEnabled(!loginPageActivated);

    mRoomFavorite->setEnabled(!loginPageActivated);
    if (mContextStatusMenu) {
        mContextStatusMenu->menuAction()->setVisible(!loginPageActivated);
    }
    if (mShowDatabaseMessages) {
        mShowDatabaseMessages->setEnabled(!loginPageActivated);
    }
    if (mShowPermissions) {
        mShowPermissions->setEnabled(!loginPageActivated);
    }
}

void RuqolaMainWindow::slotConfigureNotifications()
{
    KNotifyConfigWidget::configure(this);
}

void RuqolaMainWindow::slotRegisterNewUser()
{
    QPointer<RegisterUserDialog> dlg = new RegisterUserDialog(this);
    connect(dlg, &RegisterUserDialog::registerNewAccount, this, [this, dlg]() {
        mCurrentRocketChatAccount->registerNewUser(dlg->registerUserInfo());
    });
    dlg->exec();
    delete dlg;
}

void RuqolaMainWindow::slotRegisterUserSuccessed()
{
    KMessageBox::information(
        this,
        i18n("We have sent you an email to confirm your registration.\nIf you do not receive an email shortly, please come back and try again."),
        i18n("Register New User"));
}

void RuqolaMainWindow::slotConfigureMyAccount()
{
    MyAccountConfigureDialog dlg(mCurrentRocketChatAccount, this);
    dlg.initialize();
    dlg.exec();
}

void RuqolaMainWindow::slotAdministrator()
{
    AdministratorDialog dlg(mCurrentRocketChatAccount, this);
    dlg.initialize();
    dlg.exec();
}

void RuqolaMainWindow::slotAdministratorServerSettings()
{
    AdministratorSettingsDialog dlg(mCurrentRocketChatAccount, this);
    dlg.loadSettings();
    dlg.exec();
}

void RuqolaMainWindow::slotDirectory()
{
    auto dlg = new DirectoryDialog(mCurrentRocketChatAccount, this);
    dlg->fillTabs();
    dlg->show();
}

bool RuqolaMainWindow::queryClose()
{
    if (qApp->isSavingSession() || mReallyClose || !mNotification) {
        return true;
    }
    hide();
    return false;
}

void RuqolaMainWindow::slotClose()
{
    mReallyClose = true;
    RuqolaGlobalConfig::self()->setInternalVersion(ruqolaVersion);
    RuqolaGlobalConfig::self()->save();
    close();
}

void RuqolaMainWindow::createSystemTray()
{
#if !defined(Q_OS_IOS)
    if (!RuqolaGlobalConfig::self()->enableSystemTray()) {
        delete mNotification;
        mNotification = nullptr;
        return;
    }
    if (!mNotification) {
        mNotification = new Notification(this);
        auto trayMenu = mNotification->contextMenu();

        mContextStatusMenu = mNotification->contextMenu()->addMenu(i18nc("@item:inmenu Instant message presence status", "Status"));
        mContextStatusMenu->menuAction()->setVisible(false);
        trayMenu->addAction(actionCollection()->action(KStandardAction::name(KStandardAction::Preferences)));
        trayMenu->addAction(actionCollection()->action(KStandardAction::name(KStandardAction::ConfigureNotifications)));
        // Create systray to show notifications on Desktop
        connect(mNotification, &Notification::alert, this, [this]() {
            QApplication::alert(this, 0);
        });
    }
#endif
}

void RuqolaMainWindow::slotStatusChanged()
{
    // const auto currentStatusInfo = mCurrentRocketChatAccount->statusModel()->currentStatusInfo();
    User::PresenceStatus status = mStatusComboBox->status();
    QString messageStatus; // = currentStatusInfo.statusStr;
    if (status == User::PresenceStatus::Unknown) {
        QPointer<ModifyStatusDialog> dlg = new ModifyStatusDialog(this);
        const auto currentStatusInfo = mCurrentRocketChatAccount->statusModel()->currentStatusInfo();
        dlg->setMessageStatus(mCurrentRocketChatAccount->statusModel()->customText());
        dlg->setStatus(currentStatusInfo.status);
        if (dlg->exec()) {
            messageStatus = dlg->messageStatus();
            status = dlg->status();
            delete dlg;
        } else {
            mStatusComboBox->setStatus(mCurrentRocketChatAccount->statusModel()->currentUserStatus());
            delete dlg;
            return;
        }
    }
    mCurrentRocketChatAccount->setDefaultStatus(status, messageStatus);
}

void RuqolaMainWindow::slotUpdateStatusMenu()
{
    if (mContextStatusMenu) {
        const User::PresenceStatus status = mStatusComboBox->status();
        mContextStatusMenu->setTitle(Utils::displaytextFromPresenceStatus(status));
        mContextStatusMenu->setIcon(QIcon::fromTheme(Utils::iconFromPresenceStatus(status)));
    }
}

void RuqolaMainWindow::slotUpdateCustomUserStatus()
{
    mStatusProxyModel->sort(0);

    if (mContextStatusMenu) {
        // mContextStatusMenu->menuAction()->setVisible(true);
        mContextStatusMenu->clear();

        for (int i = 0; i < mStatusProxyModel->rowCount(); i++) {
            const QModelIndex index = mStatusProxyModel->index(i, 0);
            QAction *action = mContextStatusMenu->addAction(index.data(Qt::DisplayRole).toString());
            action->setIcon(index.data(Qt::DecorationRole).value<QIcon>());

            connect(action, &QAction::triggered, this, [this, index] {
                mStatusComboBox->setStatus(index.data(StatusModel::StatusRoles::Status).value<User::PresenceStatus>());
            });
        }
    }
}

void RuqolaMainWindow::slotHistorySwitchChannel(const QByteArray &identifier)
{
    Q_EMIT mCurrentRocketChatAccount->selectRoomByRoomIdRequested(identifier);
}

void RuqolaMainWindow::undoSwitchChannel()
{
    mSwitchChannelTreeManager->selectForward();
}

void RuqolaMainWindow::redoSwitchChannel()
{
    mSwitchChannelTreeManager->selectBackward();
}

void RuqolaMainWindow::slotToggleMenubar(bool dontShowWarning)
{
    if (menuBar()) {
        if (mShowMenuBarAction->isChecked()) {
            menuBar()->show();
        } else {
            if (!dontShowWarning && (!toolBar()->isVisible() || !toolBar()->actions().contains(mHamburgerMenu))) {
                const QString accel = mShowMenuBarAction->shortcut().toString(QKeySequence::NativeText);
                KMessageBox::information(this,
                                         i18n("<qt>This will hide the menu bar completely."
                                              " You can show it again by typing %1.</qt>",
                                              accel),
                                         i18n("Hide menu bar"),
                                         QStringLiteral("HideMenuBarWarning"));
            }
            menuBar()->hide();
        }
        RuqolaGlobalConfig::self()->setShowMenuBar(mShowMenuBarAction->isChecked());
        RuqolaGlobalConfig::self()->save();
    }
}

void RuqolaMainWindow::updateHamburgerMenu()
{
    delete mHamburgerMenu->menu();
    auto menu = new QMenu(this);
    menu->addAction(actionCollection()->action(QStringLiteral("add_server")));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(QStringLiteral("configure_my_account")));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(QStringLiteral("directory")));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(QStringLiteral("logout")));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(KStandardAction::name(KStandardAction::Quit)));
    mHamburgerMenu->setMenu(menu);
}

void RuqolaMainWindow::slotFullScreen(bool t)
{
    KToggleFullScreenAction::setFullScreen(this, t);
    QMenuBar *mb = menuBar();
    if (t) {
        auto b = new QToolButton(mb);
        b->setDefaultAction(mShowFullScreenAction);
        b->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored));
        b->setFont(QFontDatabase::systemFont(QFontDatabase::SmallestReadableFont));
        mb->setCornerWidget(b, Qt::TopRightCorner);
        b->setVisible(true);
        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    } else {
        QWidget *w = mb->cornerWidget(Qt::TopRightCorner);
        if (w) {
            w->deleteLater();
        }
    }
}

void RuqolaMainWindow::slotShowNotifyMessage(const QString &accountName, const QByteArray &messageId, const QByteArray &roomId)
{
    Ruqola::self()->setCurrentAccount(accountName);
    Q_EMIT mCurrentRocketChatAccount->selectRoomByRoomIdRequested(roomId, messageId);
}

void RuqolaMainWindow::slotOpenNotificationHistory()
{
    mNotificationToolButton->hide();
    NotificationHistoryDialog dlg(this);
    dlg.addServerList(Ruqola::self()->accountManager()->accountDisplayInfoSorted());
    connect(&dlg, &NotificationHistoryDialog::showNotifyMessage, this, &RuqolaMainWindow::slotShowNotifyMessage);
    dlg.exec();
}

void RuqolaMainWindow::slotRocketChatInformation()
{
    BannerInfoDialog dlg(mCurrentRocketChatAccount, this);
    dlg.exec();
}

void RuqolaMainWindow::slotShowRoomAvatar(bool checked)
{
    mCurrentRocketChatAccount->setShowRoomAvatar(checked);
}

void RuqolaMainWindow::slotShowFavoriteRoom(bool checked)
{
    mCurrentRocketChatAccount->setShowFavoriteRoom(checked);
}

void RuqolaMainWindow::slotImportAccounts()
{
    ImportDataWizard dlg(this);
    dlg.exec();
}

void RuqolaMainWindow::slotExportAccounts()
{
    ExportDataWizard dlg(this);
    dlg.exec();
}
#include "apps/appcategoriesjob.h"
#include "apps/appcountjob.h"
#include "apps/appfeaturedappsjob.h"
#include "apps/appmarketplacejob.h"
void RuqolaMainWindow::slotApplicationsSettings()
{
#if 0
    auto job = new RocketChatRestApi::AppCountJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::AppCountJob::appCountDone, this, [](const QJsonObject &obj) {
        qDebug() << " obj************ " << obj;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppCountJob";
    }
#endif
#if 0
    auto job = new RocketChatRestApi::AppFeaturedAppsJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::AppFeaturedAppsJob::appFeaturedAppsDone, this, [](const QJsonObject &obj) {
        qDebug() << " obj************ " << obj;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppCountJob";
    }
#endif
#if 0
    auto job = new RocketChatRestApi::AppCategoriesJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::AppCategoriesJob::appCategoriesDone, this, [](const QJsonObject &obj) {
        qDebug() << " obj************ " << obj;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start appCategories";
    }
#endif
#if 1
    auto job = new RocketChatRestApi::AppMarketPlaceJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::AppMarketPlaceJob::appMarketPlaceDone, this, [](const QJsonObject &obj) {
        qDebug() << " obj************ " << obj;
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppMarketPlaceJob";
    }
#endif
}
#include "moc_ruqolamainwindow.cpp"
