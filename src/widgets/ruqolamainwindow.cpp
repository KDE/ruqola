/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolamainwindow.h"

#include "administratordialog/administratordialog.h"
#include "administratorsettingsdialog/administratorsettingsdialog.h"
#include "applicationssettingsdialog/applicationssettingsdialog.h"
#include "databasedialog/exploredatabasedialog.h"
#include "directmessage/createdmjob.h"
#include "explorepermissionsdialog/explorepermissionsdialog.h"
#include "misc/changefontsizemenu.h"
#include "notificationhistorymanager.h"
#include "rocketchaturlutils.h"
#include "ruqolaglobalconfig.h"
#include "ruqolalogger.h"
#include "ruqolawidgets_debug.h"
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
#include "exploreappstranlationdialog/exploreappstranslationdialog.h"
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
#include "ruqolacommandlineparser.h"
#include "ruqolaserverconfig.h"
#include "switchchannelhistory/switchchanneltreeviewmanager.h"
#include "teams/teamscreatejob.h"
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
#include "whatsnew/whatsnewtranslations.h"
#include <TextAddonsWidgets/WhatsNewDialog>
#else
#include "whatsnew/whatsnewdialog.h"
#endif

#include "rooms/roomstartdiscussionjob.h"
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

#include <KColorSchemeMenu>

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
#include <KWindowSystem>
#endif
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
#include <TextAddonsWidgets/VerifyNewVersionWidget>
#else
#include "verifynewversionwidget/verifynewversionwidgetaction.h"
#endif
#endif

#if HAVE_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/NotificationPopup>
#include <KUserFeedback/Provider>
#endif
#include "colorsandmessageviewstyle.h"

namespace
{
const char myRuqolaMainWindowGroupName[] = "RuqolaMainWindow";
const int ruqolaVersion = 3;
}
using namespace Qt::Literals::StringLiterals;
RuqolaMainWindow::RuqolaMainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
    , mMainWidget(new RuqolaCentralWidget(this))
    , mStatusProxyModel(new StatusModelFilterProxyModel(this))
    , mSwitchChannelTreeManager(new SwitchChannelTreeViewManager(this))
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
    , mVerifyNewVersionWidgetAction(new TextAddonsWidgets::VerifyNewVersionWidget(this))
#else
    , mVerifyNewVersionWidgetAction(new VerifyNewVersionWidgetAction(this))
#endif
#endif

{
    Ruqola::self()->setParentWidget(mMainWidget);
    mMainWidget->setObjectName(u"mMainWidget"_s);
    connect(mMainWidget, &RuqolaCentralWidget::loginPageActivated, this, &RuqolaMainWindow::slotDisableActions);
    connect(mMainWidget, &RuqolaCentralWidget::createNewAccount, this, &RuqolaMainWindow::slotAddServer);
    setCentralWidget(mMainWidget);
    setupActions();
    setupStatusBar();
    setupGUI(/*u":/kxmlgui5/ruqola/ruqolaui.rc"_s*/);
    readConfig();
    createSystemTray();
    mSwitchChannelTreeManager->setParentWidget(mMainWidget);
    connect(mSwitchChannelTreeManager, &SwitchChannelTreeViewManager::switchToChannel, this, &RuqolaMainWindow::slotHistorySwitchChannel);
    mAccountManager = Ruqola::self()->accountManager();
    connect(mAccountManager, &AccountManager::currentAccountChanged, this, &RuqolaMainWindow::slotAccountChanged);
    connect(mAccountManager, &AccountManager::updateNotification, this, &RuqolaMainWindow::updateNotification);
    connect(mAccountManager, &AccountManager::roomNeedAttention, this, &RuqolaMainWindow::slotRoomNeedAttention);
    connect(mAccountManager, &AccountManager::logoutAccountDone, this, &RuqolaMainWindow::logout);

    connect(Ruqola::self(), &Ruqola::addInviteServer, this, &RuqolaMainWindow::slotAddInviteServer);

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
    if (parser->isSet(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::MessageUrl))) {
        const QString messageUrl = parser->value(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::MessageUrl));
        if (!messageUrl.isEmpty()) {
            if (RocketChatUrlUtils::parseUrl(messageUrl)) {
                return;
            }
        }
    }
    if (parser->isSet(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::Account))) {
        const QString loadAccount = parser->value(RuqolaCommandLineParser::commandLineFromEnum(RuqolaCommandLineParser::CommandLineName::Account));
        if (!loadAccount.isEmpty()) {
            Ruqola::self()->setCurrentAccount(loadAccount);
        }
    }
}

void RuqolaMainWindow::slotActivateRequested(const QStringList &arguments, [[maybe_unused]] const QString &workingDirectory)
{
    if (!arguments.isEmpty()) {
        QCommandLineParser parser;
        RuqolaCommandLineParser commandLineParser(&parser);
        parser.parse(arguments);
        parseCommandLine(&parser);
    }
#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
    KWindowSystem::updateStartupId(windowHandle());
    KWindowSystem::activateWindow(windowHandle());
#else
    activateWindow();
#endif
}

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
    mStatusBarTypingMessage->setObjectName(u"mStatusBarTypingMessage"_s);
    statusBar()->addPermanentWidget(mStatusBarTypingMessage, 1);
    mAccountOverviewWidget = new AccountsOverviewWidget(this);
    mAccountOverviewWidget->setObjectName(u"mAccountOverviewWidget"_s);
    statusBar()->addPermanentWidget(mAccountOverviewWidget);
    mNotificationToolButton = new QToolButton(this);
    mNotificationToolButton->setIcon(QIcon::fromTheme(u"notifications"_s));
    mNotificationToolButton->setObjectName(u"mNotificationToolButton"_s);
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
    if (mCurrentRocketChatAccount) {
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
    }
    if (!mCurrentRocketChatAccount) {
        slotDisableActions(true);
    }
    updateActions();
    slotClearNotification(); // Clear notification when we switch too.
    if (mCurrentRocketChatAccount) {
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
    mUnreadOnTop->setChecked(mCurrentRocketChatAccount && mCurrentRocketChatAccount->ownUserPreferences().showUnread());
    const auto roomListSortOrder =
        mCurrentRocketChatAccount ? mCurrentRocketChatAccount->ownUserPreferences().roomListSortOrder() : OwnUserPreferences::RoomListSortOrder::Unknown;
    mRoomListSortByLastMessage->setChecked(roomListSortOrder == OwnUserPreferences::RoomListSortOrder::ByLastMessage);
    mRoomListSortAlphabetically->setChecked(roomListSortOrder == OwnUserPreferences::RoomListSortOrder::Alphabetically);

    const auto roomListDisplay =
        mCurrentRocketChatAccount ? mCurrentRocketChatAccount->ownUserPreferences().roomListDisplay() : OwnUserPreferences::RoomListDisplay::Unknown;
    mRoomListDisplayMedium->setChecked(roomListDisplay == OwnUserPreferences::RoomListDisplay::Medium);
    mRoomListDisplayCondensed->setChecked(roomListDisplay == OwnUserPreferences::RoomListDisplay::Condensed);
    mRoomListDisplayExtended->setChecked(roomListDisplay == OwnUserPreferences::RoomListDisplay::Extended);

    mRegisterNewUser->setVisible(mCurrentRocketChatAccount && mCurrentRocketChatAccount->ruqolaServerConfig()->registrationFormEnabled());
    mCreateDiscussion->setEnabled(mCurrentRocketChatAccount && mCurrentRocketChatAccount->ruqolaServerConfig()->discussionEnabled()
                                  && (mCurrentRocketChatAccount->loginStatus() == AuthenticationManager::LoggedIn));
    const bool isAdministrator{mCurrentRocketChatAccount && mCurrentRocketChatAccount->isAdministrator()};
    mAdministrator->setVisible(isAdministrator);
    mAdministratorServerSettings->setVisible(isAdministrator);
    mShowRocketChatServerInfo->setVisible(hasBannerInfo());
    mRoomAvatar->setChecked(mCurrentRocketChatAccount && mCurrentRocketChatAccount->ownUserPreferences().showRoomAvatar());
    mRoomFavorite->setChecked(mCurrentRocketChatAccount && mCurrentRocketChatAccount->ownUserPreferences().showFavorite());
    mCreateNewChannel->setEnabled(canCreateChannels());
    mCreateDirectMessages->setEnabled(canCreateDirectMessages());
    mCreateTeam->setEnabled(canCreateTeams());
    mRequestedApplications->setVisible(isAdministrator);
}

bool RuqolaMainWindow::canCreateChannels() const
{
    return mCurrentRocketChatAccount && mCurrentRocketChatAccount->hasPermission(u"create-c"_s);
}

bool RuqolaMainWindow::canCreateDirectMessages() const
{
    return mCurrentRocketChatAccount && mCurrentRocketChatAccount->hasPermission(u"create-d"_s);
}

bool RuqolaMainWindow::canCreateTeams() const
{
    return mCurrentRocketChatAccount && mCurrentRocketChatAccount->hasPermission(u"create-team"_s);
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

    KStandardActions::quit(this, &RuqolaMainWindow::slotClose, ac);
    KStandardActions::preferences(this, &RuqolaMainWindow::slotConfigure, ac);
    KStandardActions::configureNotifications(this, &RuqolaMainWindow::slotConfigureNotifications, ac);

    auto act = new QAction(i18nc("@action", "Add Server…"), this);
    connect(act, &QAction::triggered, this, &RuqolaMainWindow::slotAddServer);
    ac->addAction(u"add_server"_s, act);

    // Move in specific server widget
    mServerInfo = new QAction(i18nc("@action", "Server Info…"), this);
    connect(mServerInfo, &QAction::triggered, this, &RuqolaMainWindow::slotServerInfo);
    ac->addAction(u"server_info"_s, mServerInfo);

    mLogout = new QAction(i18nc("@action", "Logout Current Server"), this);
    connect(mLogout, &QAction::triggered, this, &RuqolaMainWindow::slotLogout);
    ac->addAction(u"logout"_s, mLogout);

    mNextUnreadChannel = new QAction(i18nc("@action", "Jump to Next Unread Channel"), this);
    connect(mNextUnreadChannel, &QAction::triggered, this, &RuqolaMainWindow::slotSelectNextUnreadChannel);
    ac->setDefaultShortcut(mNextUnreadChannel, Qt::Key_Control | Qt::Key_PageDown);
    ac->addAction(u"next_unread_channel"_s, mNextUnreadChannel);

    mCreateNewChannel = new QAction(QIcon::fromTheme(u"irc-join-channel"_s), i18n("Create New Channel…"), this);
    connect(mCreateNewChannel, &QAction::triggered, this, &RuqolaMainWindow::slotCreateNewChannel);
    ac->addAction(u"create_new_channel"_s, mCreateNewChannel);

    mCreateDirectMessages = new QAction(QIcon::fromTheme(u"irc-join-channel"_s), i18n("Create Direct Messages…"), this);
    connect(mCreateDirectMessages, &QAction::triggered, this, &RuqolaMainWindow::slotCreateDirectMessages);
    ac->addAction(u"create_direct_messages"_s, mCreateDirectMessages);

    mCreateDiscussion = new QAction(QIcon::fromTheme(u"irc-join-channel"_s), i18n("Create Discussion…"), this);
    connect(mCreateDiscussion, &QAction::triggered, this, &RuqolaMainWindow::slotCreateDiscussion);
    ac->addAction(u"create_discussion"_s, mCreateDiscussion);

    mCreateTeam = new QAction(QIcon::fromTheme(u"irc-join-channel"_s), i18n("Create Team…"), this);
    connect(mCreateTeam, &QAction::triggered, this, &RuqolaMainWindow::slotCreateTeam);
    ac->addAction(u"create_team"_s, mCreateTeam);

    mServerMenu = new ServerMenu(this);
    mServerMenu->setActionCollection(ac);
    ac->addAction(u"server_menu"_s, mServerMenu);

    mUnreadOnTop = new QAction(i18nc("@action", "Unread on Top"), this);
    mUnreadOnTop->setCheckable(true);
    connect(mUnreadOnTop, &QAction::triggered, this, &RuqolaMainWindow::slotUnreadOnTop);
    ac->addAction(u"unread_on_top"_s, mUnreadOnTop);

    auto roomListSortOrder = new QActionGroup(this);
    roomListSortOrder->setExclusive(true);

    mRoomListSortByLastMessage = new QAction(i18nc("@action", "By Last Message"), this);
    mRoomListSortByLastMessage->setCheckable(true);
    connect(mRoomListSortByLastMessage, &QAction::triggered, this, &RuqolaMainWindow::slotRoomListSortByLastMessage);
    roomListSortOrder->addAction(mRoomListSortByLastMessage);
    ac->addAction(u"room_list_sort_by_last_message"_s, mRoomListSortByLastMessage);

    mRoomListSortAlphabetically = new QAction(i18nc("@action", "Alphabetically"), this);
    mRoomListSortAlphabetically->setCheckable(true);
    connect(mRoomListSortAlphabetically, &QAction::triggered, this, &RuqolaMainWindow::slotRoomListSortAlphabetically);
    roomListSortOrder->addAction(mRoomListSortAlphabetically);
    ac->addAction(u"room_list_sort_alphabetically"_s, mRoomListSortAlphabetically);

    mShowLog = new QAction(QIcon::fromTheme(u"view-history"_s), i18n("Show Channel Log"), this);
    connect(mShowLog, &QAction::triggered, this, &RuqolaMainWindow::slotShowLog);
    ac->addAction(u"show_log"_s, mShowLog);

    mShowServerInfo = new QAction(QIcon::fromTheme(u"data-error"_s), i18n("Show Server Errors"), this);
    connect(mShowServerInfo, &QAction::triggered, this, &RuqolaMainWindow::slotShowServerInfo);
    ac->addAction(u"show_server_errors"_s, mShowServerInfo);

    if (Ruqola::self()->debug()) {
        mMenuDebug = new QAction(u"Debug"_s, this);
        ac->addAction(u"debug_menu"_s, mMenuDebug);
        auto menu = new QMenu(this);
        mMenuDebug->setMenu(menu);
        mShowDatabaseMessages = new QAction(u"Show Database Messages…"_s, this);
        connect(mShowDatabaseMessages, &QAction::triggered, this, &RuqolaMainWindow::slotShowDatabaseMessages);
        ac->addAction(u"show_database_messages"_s, mShowDatabaseMessages);
        menu->addAction(mShowDatabaseMessages);

        menu->addSeparator();
        mShowPermissions = new QAction(u"Show Permissions…"_s, this);
        connect(mShowPermissions, &QAction::triggered, this, &RuqolaMainWindow::slotShowPermissions);
        ac->addAction(u"show_permissions"_s, mShowPermissions);
        menu->addAction(mShowPermissions);

        menu->addSeparator();
        mShowAppsTranslation = new QAction(u"Show Application Translation…"_s, this);
        connect(mShowAppsTranslation, &QAction::triggered, this, &RuqolaMainWindow::slotShowAppsTranslation);
        ac->addAction(u"show_apps_translation"_s, mShowAppsTranslation);
        menu->addAction(mShowAppsTranslation);

        menu->addSeparator();
        mShowLogsFile = new QAction(u"Show Ruqola Logs…"_s, this);
        connect(mShowLogsFile, &QAction::triggered, this, &RuqolaMainWindow::slotShowLogsFile);
        ac->addAction(u"show_ruqola_logs"_s, mShowLogsFile);
        menu->addAction(mShowLogsFile);

        menu->addSeparator();
        mShowRestApiLogsFile = new QAction(u"Show Ruqola Logs (RESTAPI)…"_s, this);
        connect(mShowRestApiLogsFile, &QAction::triggered, this, &RuqolaMainWindow::slotShowRestApiLogsFile);
        ac->addAction(u"show_ruqola_restapi_logs"_s, mShowRestApiLogsFile);
        menu->addAction(mShowRestApiLogsFile);

        menu->addSeparator();
        mShowDatabaseLogsFile = new QAction(u"Show Ruqola Database Logs…"_s, this);
        connect(mShowDatabaseLogsFile, &QAction::triggered, this, &RuqolaMainWindow::slotShowDatabaseLogsFile);
        ac->addAction(u"show_ruqola_database_logs"_s, mShowDatabaseLogsFile);
        menu->addAction(mShowDatabaseLogsFile);
    }

    mClearAlerts = new QAction(i18nc("@action", "Mark All Channels as Read"), this);
    ac->setDefaultShortcut(mClearAlerts, Qt::SHIFT | Qt::Key_Escape);
    connect(mClearAlerts, &QAction::triggered, this, &RuqolaMainWindow::slotClearAccountAlerts);
    ac->addAction(u"mark_all_channels_read"_s, mClearAlerts);

    mRegisterNewUser = new QAction(i18nc("@action", "Register a New User…"), this);
    connect(mRegisterNewUser, &QAction::triggered, this, &RuqolaMainWindow::slotRegisterNewUser);
    ac->addAction(u"register_new_user"_s, mRegisterNewUser);

    mMyAccount = new QAction(i18nc("@action", "My Account…"), this);
    connect(mMyAccount, &QAction::triggered, this, &RuqolaMainWindow::slotConfigureMyAccount);
    ac->addAction(u"configure_my_account"_s, mMyAccount);

    mAdministrationMenu = new KActionMenu(i18n("administration"), this);
    mAdministrator = new QAction(i18nc("@action", "Administrator…"), this);
    ac->addAction(u"administrator_account_settings"_s, mAdministrator);
    connect(mAdministrator, &QAction::triggered, this, &RuqolaMainWindow::slotAdministrator);
    mAdministrationMenu->addAction(mAdministrator);
    ac->addAction(u"administration"_s, mAdministrationMenu);

    mAdministratorServerSettings = new QAction(i18nc("@action", "Server Settings…"), this);
    connect(mAdministratorServerSettings, &QAction::triggered, this, &RuqolaMainWindow::slotAdministratorServerSettings);
    ac->addAction(u"administrator_server_settings"_s, mAdministratorServerSettings);
    mAdministrationMenu->addAction(mAdministratorServerSettings);

    mAdministrationMenu->addSeparator();

    mApplications = new QAction(i18nc("@action", "Applications…"), this);
    connect(mApplications, &QAction::triggered, this, &RuqolaMainWindow::slotApplicationsSettings);
    ac->addAction(u"applications_settings"_s, mApplications);
    mAdministrationMenu->addAction(mApplications);

    mRequestedApplications = new QAction(i18nc("@action", "Requested Applications…"), this);
    connect(mRequestedApplications, &QAction::triggered, this, &RuqolaMainWindow::slotApplicationsRequestedSettings);
    ac->addAction(u"applications_requested_settings"_s, mRequestedApplications);
    mAdministrationMenu->addAction(mRequestedApplications);

    mInstalledApplications = new QAction(i18nc("@action", "Installed Applications…"), this);
    connect(mInstalledApplications, &QAction::triggered, this, &RuqolaMainWindow::slotApplicationsInstalledSettings);
    ac->addAction(u"applications_requested_settings"_s, mInstalledApplications);
    mAdministrationMenu->addAction(mInstalledApplications);

    mPrivateApplications = new QAction(i18nc("@action", "Private Applications…"), this);
    connect(mPrivateApplications, &QAction::triggered, this, &RuqolaMainWindow::slotPrivateApplicationsSettings);
    ac->addAction(u"applications_private_settings"_s, mPrivateApplications);
    mAdministrationMenu->addAction(mPrivateApplications);

    mJoinRoom = new QAction(i18nc("Action which allows to search room", "Join Room…"), this);
    connect(mJoinRoom, &QAction::triggered, this, &RuqolaMainWindow::slotJoinRoom);
    ac->addAction(u"join_room"_s, mJoinRoom);

    mJoinTeam = new QAction(i18nc("Action which allows to search team", "Join Team…"), this);
    connect(mJoinTeam, &QAction::triggered, this, &RuqolaMainWindow::slotJoinTeam);
    ac->addAction(u"join_team"_s, mJoinTeam);

    mOpenPrivateConversation = new QAction(i18nc("Action which allows to open private conversation (direct user)", "Open Private Conversation…"), this);
    connect(mOpenPrivateConversation, &QAction::triggered, this, &RuqolaMainWindow::slotOpenPrivateChannel);
    ac->addAction(u"open_private_conversation"_s, mOpenPrivateConversation);

    // Actions to navigate through the different pages
    QList<QKeySequence> nextShortcut;
    QList<QKeySequence> prevShortcut;

    QString nextIcon, prevIcon;
    if (QApplication::isRightToLeft()) {
        prevShortcut.append(QKeySequence(u"Alt+Right"_s));
        nextShortcut.append(QKeySequence(u"Alt+Left"_s));
        nextIcon = u"go-previous-view"_s;
        prevIcon = u"go-next-view"_s;
    } else {
        nextShortcut.append(QKeySequence(u"Alt+Right"_s));
        prevShortcut.append(QKeySequence(u"Alt+Left"_s));
        nextIcon = u"go-next-view"_s;
        prevIcon = u"go-previous-view"_s;
    }

    mNextTab = new QAction(QIcon::fromTheme(nextIcon), i18n("&Next Server"), this);
    actionCollection()->setDefaultShortcuts(mNextTab, nextShortcut);
    connect(mNextTab, &QAction::triggered, this, &RuqolaMainWindow::showNextView);
    actionCollection()->addAction(u"next_tab"_s, mNextTab);

    mPreviewTab = new QAction(QIcon::fromTheme(prevIcon), i18n("&Previous Server"), this);
    actionCollection()->setDefaultShortcuts(mPreviewTab, prevShortcut);
    connect(mPreviewTab, &QAction::triggered, this, &RuqolaMainWindow::showPreviousView);
    actionCollection()->addAction(u"previous_tab"_s, mPreviewTab);

    {
        mContainerStatusInfo = new QWidget(this);
        auto layout = new QHBoxLayout(mContainerStatusInfo);
        layout->setContentsMargins({});
        auto label = new QLabel(i18nc("@label:textbox", "Status:"), mContainerStatusInfo);
        label->setObjectName(u"label"_s);
        layout->addWidget(label);

        mStatusComboBox = new StatusCombobox(mContainerStatusInfo);
        mStatusComboBox->setObjectName(u"mStatusComboBox"_s);
        layout->addWidget(mStatusComboBox);
        connect(mStatusComboBox, &StatusCombobox::currentIndexChanged, this, &RuqolaMainWindow::slotStatusChanged);
        connect(mStatusComboBox, &StatusCombobox::currentIndexChanged, this, &RuqolaMainWindow::slotUpdateStatusMenu);
    }

    {
        QList<QAction *> listActions;
        auto actSelectChanel = new QAction(i18nc("@action", "Previous Selected Channel"), this);
        ac->setDefaultShortcut(actSelectChanel, QKeySequence(Qt::CTRL | Qt::Key_Tab));
        ac->addAction(u"previous_channel"_s, actSelectChanel);
        listActions.append(actSelectChanel);

        connect(actSelectChanel, &QAction::triggered, this, &RuqolaMainWindow::undoSwitchChannel);

        actSelectChanel = new QAction(i18nc("@action", "Next Selected Channel"), this);
        ac->addAction(u"next_channel"_s, actSelectChanel);
        const QKeyCombination combinationKeys(Qt::CTRL | Qt::SHIFT, Qt::Key_Tab);
        ac->setDefaultShortcut(actSelectChanel, combinationKeys);
        connect(actSelectChanel, &QAction::triggered, this, &RuqolaMainWindow::redoSwitchChannel);
        listActions.append(actSelectChanel);

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
    auto manager = KColorSchemeManager::instance();
    ac->addAction(u"colorscheme_menu"_s, KColorSchemeMenu::createMenu(manager, this));

    mShowFullScreenAction = KStandardAction::fullScreen(nullptr, nullptr, this, ac);
    ac->setDefaultShortcut(mShowFullScreenAction, Qt::Key_F11);
    connect(mShowFullScreenAction, &QAction::toggled, this, &RuqolaMainWindow::slotFullScreen);

    mShowNotifyHistory = new QAction(QIcon::fromTheme(u"notifications"_s), i18n("Show Notify History"), this);
    connect(mShowNotifyHistory, &QAction::triggered, this, &RuqolaMainWindow::slotOpenNotificationHistory);
    ac->addAction(u"show_notify_history"_s, mShowNotifyHistory);

    mShowRocketChatServerInfo = new QAction(i18nc("@action", "Show RocketChat Information"), this);
    connect(mShowRocketChatServerInfo, &QAction::triggered, this, &RuqolaMainWindow::slotRocketChatInformation);
    ac->addAction(u"show_rocketchat_information"_s, mShowRocketChatServerInfo);

    mRoomAvatar = new QAction(i18nc("@action", "Show Room Avatar"), this);
    mRoomAvatar->setCheckable(true);
    connect(mRoomAvatar, &QAction::triggered, this, &RuqolaMainWindow::slotShowRoomAvatar);
    ac->addAction(u"room_avatar"_s, mRoomAvatar);

    mRoomFavorite = new QAction(i18nc("@action", "Show Favorite Room"), this);
    mRoomFavorite->setCheckable(true);
    connect(mRoomFavorite, &QAction::triggered, this, &RuqolaMainWindow::slotShowFavoriteRoom);
    ac->addAction(u"room_favorite"_s, mRoomFavorite);
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    auto verifyNewVersionAction = mVerifyNewVersionWidgetAction->verifyNewVersionAction();
    ac->addAction(u"verify_check_version"_s, verifyNewVersionAction);
#endif

    auto showWhatsNewAction = new QAction(QIcon(u":/ruqola/ruqola.svg"_s), i18n("What's new"), this);
    ac->addAction(u"whatsnew_ruqola"_s, showWhatsNewAction);
    connect(showWhatsNewAction, &QAction::triggered, this, &RuqolaMainWindow::slotWhatsNew);

    mImportAccountsAction = new QAction(QIcon::fromTheme(u"document-import"_s), i18n("Import Accounts"), this);
    connect(mImportAccountsAction, &QAction::triggered, this, &RuqolaMainWindow::slotImportAccounts);
    ac->addAction(u"import_accounts"_s, mImportAccountsAction);

    mExportAccountsAction = new QAction(QIcon::fromTheme(u"document-export"_s), i18n("Export Accounts"), this);
    connect(mExportAccountsAction, &QAction::triggered, this, &RuqolaMainWindow::slotExportAccounts);
    ac->addAction(u"export_accounts"_s, mExportAccountsAction);

    mMessageStyleAction = new MessageStyleLayoutMenu(this);
    ac->addAction(u"message_style"_s, mMessageStyleAction);
    connect(mMessageStyleAction, &MessageStyleLayoutMenu::styleChanged, this, &RuqolaMainWindow::slotMessageStyleChanged);

    mChangeFontSizeAction = new ChangeFontSizeMenu(this);
    ac->addAction(u"change_font_size"_s, mChangeFontSizeAction);
    connect(mChangeFontSizeAction, &ChangeFontSizeMenu::fontChanged, this, [] {
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
    ac->addAction(u"room_list_display_medium"_s, mRoomListDisplayMedium);

    mRoomListDisplayCondensed = new QAction(i18nc("@action", "Condensed"), this);
    mRoomListDisplayCondensed->setCheckable(true);
    connect(mRoomListDisplayCondensed, &QAction::triggered, this, [this]() {
        mCurrentRocketChatAccount->setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Condensed);
    });

    roomListDisplay->addAction(mRoomListDisplayCondensed);
    ac->addAction(u"room_list_display_condensed"_s, mRoomListDisplayCondensed);

    mRoomListDisplayExtended = new QAction(i18nc("@action", "Extended"), this);
    mRoomListDisplayExtended->setCheckable(true);
    connect(mRoomListDisplayExtended, &QAction::triggered, this, [this]() {
        mCurrentRocketChatAccount->setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Extended);
    });
    roomListDisplay->addAction(mRoomListDisplayExtended);
    ac->addAction(u"room_list_display_extended"_s, mRoomListDisplayExtended);

    mClearRoomHistory = new QAction(QIcon::fromTheme(u"edit-clear-all"_s), i18n("Clear history"), this);
    connect(mClearRoomHistory, &QAction::triggered, this, &RuqolaMainWindow::slotClearRoomHistory);
    ac->addAction(u"clear_room_history"_s, mClearRoomHistory);

#if ADD_OFFLINE_SUPPORT
    mOfflineMode = new QAction(i18n("Work Offline"), this);
    connect(mOfflineMode, &QAction::triggered, this, &RuqolaMainWindow::slotWorkOfflineMode);
    ac->addAction(u"work_offline"_s, mOfflineMode);
#endif
}

void RuqolaMainWindow::slotMessageStyleChanged()
{
    Q_EMIT ColorsAndMessageViewStyle::self().needUpdateMessageStyle();
}

void RuqolaMainWindow::slotWhatsNew()
{
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
    WhatsNewTranslations translations;
    TextAddonsWidgets::WhatsNewDialog dlg(translations.createWhatsNewInfo(), this);
#else
    WhatsNewDialog dlg(this);
#endif
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
    if (mCurrentRocketChatAccount->ruqolaServerConfig()->broadCastEnabled()) {
        flags |= CreateNewChannelWidget::Feature::BroadCast;
    }
    if (mCurrentRocketChatAccount->ruqolaServerConfig()->encryptionEnabled()) {
        flags |= CreateNewChannelWidget::Feature::Encrypted;
    }
    if (mCurrentRocketChatAccount->ruqolaServerConfig()->federationEnabled()) {
        flags |= CreateNewChannelWidget::Feature::Federated;
    }
    dlg->setFeatures(flags);
    if (dlg->exec()) {
        RocketChatRestApi::CreateChannelTeamInfo teamInfo = dlg->channelInfo(true);
        teamInfo.infoType = RocketChatRestApi::CreateChannelTeamInfo::CreateInfoType::Team;
        auto job = new RocketChatRestApi::TeamsCreateJob(this);
        job->setTeamsCreateJobInfo(teamInfo);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::TeamsCreateJob::teamCreateDone, this, [this](const QJsonObject &replyObject) {
            mCurrentRocketChatAccount->extractIdentifier(replyObject, "team"_L1, "_id"_L1);
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamsCreateJob";
        }
    }
    delete dlg;
}

void RuqolaMainWindow::slotCreateDiscussion()
{
    QPointer<CreateNewDiscussionDialog> dlg = new CreateNewDiscussionDialog(mCurrentRocketChatAccount, this);
    if (dlg->exec()) {
        const CreateNewDiscussionDialog::NewDiscussionInfo info = dlg->newDiscussionInfo();
        auto job = new RocketChatRestApi::RoomStartDiscussionJob(this);
        connect(job, &RocketChatRestApi::RoomStartDiscussionJob::startDiscussionDone, this, [this](const QJsonObject &replyObj) {
            mCurrentRocketChatAccount->extractIdentifier(replyObj, "discussion"_L1, "_id"_L1);
        });
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        job->setParentRoomId(info.channelId);

        job->setDiscussionName(info.discussionName);
        job->setParentMessageId(dlg->messageId());
        job->setReplyMessage(info.message);
        job->setUsers(info.users);
        if (!job->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start RoomStartDiscussionJob";
        }
    }
    delete dlg;
}

void RuqolaMainWindow::slotCreateDirectMessages()
{
    QPointer<CreateDirectMessagesDialog> dlg = new CreateDirectMessagesDialog(mCurrentRocketChatAccount, this);
    if (dlg->exec()) {
        const QStringList usernames = dlg->userNames();
        auto job = new RocketChatRestApi::CreateDmJob(this);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::CreateDmJob::createDmDone, this, [this](const QJsonObject &replyObject) {
            mCurrentRocketChatAccount->extractIdentifier(replyObject, "room"_L1, "_id"_L1);
        });
        job->setUserNames(usernames);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start createDirectMessage job";
        }
    }
    delete dlg;
}

void RuqolaMainWindow::slotCreateNewChannel()
{
    QPointer<CreateNewChannelDialog> dlg = new CreateNewChannelDialog(mCurrentRocketChatAccount, this);
    CreateNewChannelWidget::Features flags;
    if (mCurrentRocketChatAccount->ruqolaServerConfig()->broadCastEnabled()) {
        flags |= CreateNewChannelWidget::Feature::BroadCast;
    }
    if (mCurrentRocketChatAccount->ruqolaServerConfig()->encryptionEnabled()) {
        flags |= CreateNewChannelWidget::Feature::Encrypted;
    }
    if (mCurrentRocketChatAccount->ruqolaServerConfig()->federationEnabled()) {
        flags |= CreateNewChannelWidget::Feature::Federated;
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

void RuqolaMainWindow::slotShowAppsTranslation()
{
    ExploreAppsTranslationDialog dlg(this);
    dlg.setAppsLanguagesInfoMap(mCurrentRocketChatAccount->languagesAppsMap());
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
    auto room = mMainWidget->room();
    if (room) {
        QTemporaryFile tempFile(QDir::tempPath() + u"/XXXXXX.log.txt"_s);
        tempFile.setAutoRemove(false);
        if (tempFile.open()) {
            LocalMessageLogger logger;
            if (logger.saveToFile(tempFile, mCurrentRocketChatAccount->accountName(), room->roomId())) {
                const QString fileName = tempFile.fileName();
                tempFile.close();
                auto job = new KIO::OpenUrlJob(QUrl::fromLocalFile(fileName), this);
                job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, this));
                job->setDeleteTemporaryFile(true);
                job->start();
            } else {
                KMessageBox::information(this, i18n("Cannot write to log file %1", tempFile.fileName()), i18nc("@title:window", "Show Channel Log"));
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

void RuqolaMainWindow::slotDisableActions(bool loginPageActivated)
{
    mCreateNewChannel->setEnabled(!loginPageActivated && canCreateChannels());
    mCreateDirectMessages->setEnabled(!loginPageActivated && canCreateDirectMessages());
    mLogout->setEnabled(!loginPageActivated);
    mClearAlerts->setEnabled(!loginPageActivated);
    mMyAccount->setEnabled(!loginPageActivated);
    mContainerStatusInfo->setEnabled(!loginPageActivated);
    mCreateDiscussion->setEnabled(!loginPageActivated);
    mCreateTeam->setEnabled(!loginPageActivated && canCreateTeams());
    mJoinRoom->setEnabled(!loginPageActivated);
    mOpenPrivateConversation->setEnabled(!loginPageActivated);
    mJoinTeam->setEnabled(!loginPageActivated);
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
    mClearRoomHistory->setEnabled(!loginPageActivated);

    mApplications->setEnabled(!loginPageActivated);
    mRequestedApplications->setEnabled(!loginPageActivated);
    mInstalledApplications->setEnabled(!loginPageActivated);

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
    if (mShowAppsTranslation) {
        mShowAppsTranslation->setEnabled(!loginPageActivated);
    }
    mAdministrationMenu->setEnabled(!loginPageActivated);
    mMessageStyleAction->setEnabled(!loginPageActivated);
    mChangeFontSizeAction->setEnabled(!loginPageActivated);
    const bool enableLogsStatus = mCurrentRocketChatAccount && mCurrentRocketChatAccount->ruqolaLogger();
    if (mShowLogsFile) {
        mShowLogsFile->setEnabled(enableLogsStatus);
    }
    if (mShowRestApiLogsFile) {
        mShowRestApiLogsFile->setEnabled(enableLogsStatus);
    }
    if (mShowDatabaseLogsFile) {
        mShowDatabaseLogsFile->setEnabled(enableLogsStatus);
    }
}

void RuqolaMainWindow::slotConfigureNotifications()
{
    KNotifyConfigWidget::configure(this);
}

void RuqolaMainWindow::slotRegisterNewUser()
{
    QPointer<RegisterUserDialog> dlg = new RegisterUserDialog(this);
    if (mCurrentRocketChatAccount) {
        dlg->setPasswordValidChecks(mCurrentRocketChatAccount->ruqolaServerConfig()->passwordSettings());
        dlg->setManuallyApproveNewUsersRequired(mCurrentRocketChatAccount->ruqolaServerConfig()->accountsManuallyApproveNewUsers());
    }
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
        i18nc("@title:window", "Register New User"));
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

void RuqolaMainWindow::slotJoinRoom()
{
    auto dlg = new DirectoryDialog(mCurrentRocketChatAccount, DirectoryWidget::DirectoryType::Room, this);
    dlg->show();
}

void RuqolaMainWindow::slotJoinTeam()
{
    auto dlg = new DirectoryDialog(mCurrentRocketChatAccount, DirectoryWidget::DirectoryType::Team, this);
    dlg->show();
}

void RuqolaMainWindow::slotOpenPrivateChannel()
{
    auto dlg = new DirectoryDialog(mCurrentRocketChatAccount, DirectoryWidget::DirectoryType::User, this);
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
        trayMenu->addAction(actionCollection()->action(KStandardActions::name(KStandardActions::Preferences)));
        trayMenu->addAction(actionCollection()->action(KStandardActions::name(KStandardActions::ConfigureNotifications)));
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
            QAction *action = mContextStatusMenu->addAction(index.data(Qt::DecorationRole).value<QIcon>(), index.data(Qt::DisplayRole).toString());

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
                                         i18nc("@title:window", "Hide menu bar"),
                                         u"HideMenuBarWarning"_s);
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
    menu->addAction(actionCollection()->action(u"add_server"_s));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(u"configure_my_account"_s));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(u"directory"_s));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(u"logout"_s));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(KStandardActions::name(KStandardActions::Quit)));
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

void RuqolaMainWindow::applicationSettings(ApplicationsSettingsSearchWidget::Feature feature)
{
    ApplicationsSettingsDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setFeature(feature);
    dlg.initialize();
    dlg.exec();
}

void RuqolaMainWindow::slotApplicationsSettings()
{
    applicationSettings(ApplicationsSettingsSearchWidget::Feature::None);
}

void RuqolaMainWindow::slotApplicationsRequestedSettings()
{
    applicationSettings(ApplicationsSettingsSearchWidget::Feature::Requested);
}

void RuqolaMainWindow::slotApplicationsInstalledSettings()
{
    applicationSettings(ApplicationsSettingsSearchWidget::Feature::Installed);
}

void RuqolaMainWindow::slotClearRoomHistory()
{
    auto room = mMainWidget->room();
    if (room) {
        room->clearHistory();
    }
}

void RuqolaMainWindow::slotShowLogsFile()
{
    auto job = new KIO::OpenUrlJob(QUrl::fromLocalFile(mCurrentRocketChatAccount->ruqolaLogger()->loggerFilePath()), this);
    job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, this));
    job->setDeleteTemporaryFile(true);
    job->start();
}

void RuqolaMainWindow::slotShowRestApiLogsFile()
{
    auto job = new KIO::OpenUrlJob(QUrl::fromLocalFile(mCurrentRocketChatAccount->ruqolaLogger()->restApiLoggerFilePath()), this);
    job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, this));
    job->setDeleteTemporaryFile(true);
    job->start();
}

void RuqolaMainWindow::slotShowDatabaseLogsFile()
{
    auto job = new KIO::OpenUrlJob(QUrl::fromLocalFile(mCurrentRocketChatAccount->ruqolaLogger()->databaseLogFilePath()), this);
    job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, this));
    job->setDeleteTemporaryFile(true);
    job->start();
}

void RuqolaMainWindow::slotPrivateApplicationsSettings()
{
    applicationSettings(ApplicationsSettingsSearchWidget::Feature::Private);
}

void RuqolaMainWindow::slotAddInviteServer(const AccountManager::AccountManagerInfo &info)
{
    QPointer<CreateNewServerDialog> dlg = new CreateNewServerDialog(this);
    const QStringList lst = mAccountManager->accountsName();
    dlg->setExistingAccountName(lst);
    dlg->checkServerUrl(info.serverUrl);
    if (dlg->exec()) {
        AccountManager::AccountManagerInfo newInfo = dlg->accountInfo();
        newInfo.inviteToken = info.inviteToken;
        mAccountManager->addAccount(std::move(newInfo));
    }
    delete dlg;
}

void RuqolaMainWindow::slotWorkOfflineMode()
{
#if ADD_OFFLINE_SUPPORT
    // TODO
#endif
    // TODO
}

#include "moc_ruqolamainwindow.cpp"
