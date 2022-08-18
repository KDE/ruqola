/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolamainwindow.h"
#include "notificationhistorymanager.h"
#include "ruqolaglobalconfig.h"
#include "ruqolawidgets_debug.h"

#include "accountmanager.h"
#include "administratordialog/administratordialog.h"
#include "administratorsettingsdialog/administratorsettingsdialog.h"

#include "bannerinfodialog/bannerinfodialog.h"
#include "config-ruqola.h"
#include "configuredialog/configuresettingsdialog.h"
#include "connection.h"
#include "dialogs/channelpassworddialog.h"
#include "dialogs/createdirectmessagesdialog.h"
#include "dialogs/createnewchanneldialog.h"
#include "dialogs/createnewdiscussiondialog.h"
#include "dialogs/createnewserverdialog.h"
#include "dialogs/modifystatusdialog.h"
#include "dialogs/serverinfo/serverinfodialog.h"
#include "directory/directorydialog.h"
#include "localmessagelogger.h"
#include "misc/accountsoverviewwidget.h"
#include "misc/servermenu.h"
#include "misc/statuscombobox.h"
#include "model/statusmodel.h"
#include "model/statusmodelfilterproxymodel.h"
#include "myaccount/myaccountconfiguredialog.h"
#include "notificationhistory/notificationhistorydialog.h"
#include "notifications/notification.h"
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
#include <KActionCollection>
#include <KColorSchemeManager>
#include <KConfigGroup>
#include <KIO/JobUiDelegate>
#include <KIO/OpenUrlJob>
#include <KLocalizedString>
#include <KMessageBox>
#include <KNotifyConfigWidget>
#include <KSharedConfig>
#include <KStandardAction>
#include <KToggleFullScreenAction>
#include <KToolBar>
#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTemporaryFile>
#include <QToolButton>
#include <QWidgetAction>

#if !defined(Q_OS_WIN) && !defined(Q_OS_MACOS)
#include <KWindowSystem>
#endif

#if HAVE_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#include <KUserFeedback/NotificationPopup>
#include <KUserFeedback/Provider>
#endif

namespace
{
static const char myRuqolaMainWindowGroupName[] = "RuqolaMainWindow";
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
    connect(mAccountManager, &AccountManager::messageUrlNotFound, this, &RuqolaMainWindow::slotMessageUrlNotFound);
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
    KConfigGroup group = config->group(myRuqolaMainWindowGroupName);
    group.writeEntry("Size", size());

    delete mMainWidget; // before Ruqola::destroy()

    Ruqola::destroy();
}

void RuqolaMainWindow::parseCommandLine(QCommandLineParser *parser)
{
    if (parser->isSet(QStringLiteral("messageurl"))) {
        const QString messageUrl = parser->value(QStringLiteral("messageurl"));
        if (!messageUrl.isEmpty()) {
            Ruqola::self()->openMessageUrl(messageUrl);
        }
    }
    if (parser->isSet(QStringLiteral("account"))) {
        const QString loadAccount = parser->value(QStringLiteral("account"));
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
        ruqolaOptions(&parser);
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
    mStatusBarTypingMessage = new QLabel(this);
    mStatusBarTypingMessage->setTextFormat(Qt::RichText);
    mStatusBarTypingMessage->setObjectName(QStringLiteral("mStatusBarTypingMessage"));
    statusBar()->addPermanentWidget(mStatusBarTypingMessage);
    mAccountOverviewWidget = new AccountsOverviewWidget(this);
    mAccountOverviewWidget->setObjectName(QStringLiteral("mAccountOverviewWidget"));
    statusBar()->addPermanentWidget(mAccountOverviewWidget);
    mNotificationToolButton = new QToolButton(this);
    mNotificationToolButton->setIcon(QIcon::fromTheme(QStringLiteral("notifications")));
    mNotificationToolButton->setObjectName(QStringLiteral("mNotificationToolButton"));
    mNotificationToolButton->setToolTip(i18n("Show New Notifications"));
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
    connect(mCurrentRocketChatAccount, &RocketChatAccount::needUpdateChannelView, this, [this]() {
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
    connect(mCurrentRocketChatAccount, &RocketChatAccount::updateStatusComboBox, this, &RuqolaMainWindow::slotUpdateCustomUserStatus);

    updateActions();
    slotClearNotification(); // Clear notification when we switch too.
    mMainWidget->setCurrentRocketChatAccount(mCurrentRocketChatAccount);
    mSwitchChannelTreeManager->setChannelSwitcherModel(mCurrentRocketChatAccount->switchChannelHistoryModel());

    mStatusComboBox->blockSignals(true);
    mStatusProxyModel->setSourceModel(mCurrentRocketChatAccount->statusModel());
    mStatusComboBox->setModel(mStatusProxyModel);

    slotUpdateCustomUserStatus();
    mStatusComboBox->setStatus(mCurrentRocketChatAccount->presenceStatus());
    mStatusComboBox->blockSignals(false);
}

void RuqolaMainWindow::slotRaiseWindow()
{
    show();
    raise();
    activateWindow();
}

void RuqolaMainWindow::slotPermissionChanged()
{
    mCreateTeam->setEnabled(mCurrentRocketChatAccount->teamEnabled());
}

void RuqolaMainWindow::updateActions()
{
    mUnreadOnTop->setChecked(mCurrentRocketChatAccount->sortUnreadOnTop());
    mRegisterNewUser->setVisible(mCurrentRocketChatAccount->registrationFromEnabled());
    mCreateDiscussion->setEnabled(mCurrentRocketChatAccount->discussionEnabled());
    const bool isAdministrator{mCurrentRocketChatAccount->isAdministrator()};
    mAdministrator->setEnabled(isAdministrator);
    mAdministratorServerSettings->setEnabled(isAdministrator);
    mAdministratorMenu->setVisible(isAdministrator);
    mShowRocketChatServerInfo->setVisible(hasBannerInfo());
    mRoomAvatar->setChecked(mCurrentRocketChatAccount->ownUserPreferences().showRoomAvatar());
}

bool RuqolaMainWindow::hasBannerInfo() const
{
    return mCurrentRocketChatAccount && !mCurrentRocketChatAccount->bannerInfos().isEmpty();
}

void RuqolaMainWindow::readConfig()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group = KConfigGroup(config, myRuqolaMainWindowGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void RuqolaMainWindow::slotClearNotification()
{
    mStatusBarTypingMessage->clear();
}

void RuqolaMainWindow::slotTypingNotificationChanged(const QString &roomId, const QString &notificationStr)
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

    auto act = new QAction(i18n("Add Server..."), this);
    connect(act, &QAction::triggered, this, &RuqolaMainWindow::slotAddServer);
    ac->addAction(QStringLiteral("add_server"), act);

    // Move in specific server widget
    mServerInfo = new QAction(i18n("Server Info..."), this);
    connect(mServerInfo, &QAction::triggered, this, &RuqolaMainWindow::slotServerInfo);
    ac->addAction(QStringLiteral("server_info"), mServerInfo);

    mLogout = new QAction(i18n("Logout Current Server"), this);
    connect(mLogout, &QAction::triggered, this, &RuqolaMainWindow::slotLogout);
    ac->addAction(QStringLiteral("logout"), mLogout);

    mNextUnreadChannel = new QAction(i18n("Jump to Next Unread Channel"), this);
    connect(mNextUnreadChannel, &QAction::triggered, this, &RuqolaMainWindow::slotSelectNextUnreadChannel);
    ac->setDefaultShortcut(mNextUnreadChannel, Qt::Key_Control | Qt::Key_PageDown);
    ac->addAction(QStringLiteral("next_unread_channel"), mNextUnreadChannel);

    mCreateNewChannel = new QAction(QIcon::fromTheme(QStringLiteral("irc-join-channel")), i18n("Create New Channel..."), this);
    connect(mCreateNewChannel, &QAction::triggered, this, &RuqolaMainWindow::slotCreateNewChannel);
    ac->addAction(QStringLiteral("create_new_channel"), mCreateNewChannel);

    mCreateDirectMessages = new QAction(QIcon::fromTheme(QStringLiteral("irc-join-channel")), i18n("Create Direct Messages..."), this);
    connect(mCreateDirectMessages, &QAction::triggered, this, &RuqolaMainWindow::slotCreateDirectMessages);
    ac->addAction(QStringLiteral("create_direct_messages"), mCreateDirectMessages);

    mCreateDiscussion = new QAction(QIcon::fromTheme(QStringLiteral("irc-join-channel")), i18n("Create Discussion..."), this);
    connect(mCreateDiscussion, &QAction::triggered, this, &RuqolaMainWindow::slotCreateDiscussion);
    ac->addAction(QStringLiteral("create_discussion"), mCreateDiscussion);

    mCreateTeam = new QAction(QIcon::fromTheme(QStringLiteral("irc-join-channel")), i18n("Create Team..."), this);
    connect(mCreateTeam, &QAction::triggered, this, &RuqolaMainWindow::slotCreateTeam);
    ac->addAction(QStringLiteral("create_team"), mCreateTeam);

    mServerMenu = new ServerMenu(this);
    mServerMenu->setActionCollection(ac);
    ac->addAction(QStringLiteral("server_menu"), mServerMenu);

    mUnreadOnTop = new QAction(i18n("Unread on Top"), this);
    mUnreadOnTop->setCheckable(true);
    connect(mUnreadOnTop, &QAction::triggered, this, &RuqolaMainWindow::slotUnreadOnTop);
    ac->addAction(QStringLiteral("unread_on_top"), mUnreadOnTop);

    mShowLog = new QAction(QIcon::fromTheme(QStringLiteral("view-history")), i18n("Show Channel Log"), this);
    connect(mShowLog, &QAction::triggered, this, &RuqolaMainWindow::slotShowLog);
    ac->addAction(QStringLiteral("show_log"), mShowLog);

    mClearAlerts = new QAction(i18n("Mark All Channels as Read"), this);
    ac->setDefaultShortcut(mClearAlerts, Qt::SHIFT | Qt::Key_Escape);
    connect(mClearAlerts, &QAction::triggered, this, &RuqolaMainWindow::slotClearAccountAlerts);
    ac->addAction(QStringLiteral("mark_all_channels_read"), mClearAlerts);

    mRegisterNewUser = new QAction(i18n("Register a New User..."), this);
    connect(mRegisterNewUser, &QAction::triggered, this, &RuqolaMainWindow::slotRegisterNewUser);
    ac->addAction(QStringLiteral("register_new_user"), mRegisterNewUser);

    mMyAccount = new QAction(i18n("My Account..."), this);
    connect(mMyAccount, &QAction::triggered, this, &RuqolaMainWindow::slotConfigureMyAccount);
    ac->addAction(QStringLiteral("configure_my_account"), mMyAccount);

    mAdministratorMenu = new KActionMenu(i18n("Administrator"), this);
    mAdministrator = new QAction(i18n("Administrator..."), this);
    ac->addAction(QStringLiteral("administrator_account_settings"), mAdministrator);
    connect(mAdministrator, &QAction::triggered, this, &RuqolaMainWindow::slotAdministrator);
    mAdministratorMenu->addAction(mAdministrator);
    ac->addAction(QStringLiteral("administrator"), mAdministratorMenu);

    mAdministratorServerSettings = new QAction(i18n("Server Settings..."), this);
    connect(mAdministratorServerSettings, &QAction::triggered, this, &RuqolaMainWindow::slotAdministratorServerSettings);
    ac->addAction(QStringLiteral("administrator_server_settings"), mAdministratorServerSettings);
    mAdministratorMenu->addAction(mAdministratorServerSettings);

    mDirectory = new QAction(i18n("Directory..."), this);
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
        auto action = new QWidgetAction(this);
        action->setText(i18n("Status"));
        auto container = new QWidget(this);
        // use the same font as other toolbar buttons
        container->setFont(qApp->font("QToolButton"));
        action->setDefaultWidget(container);
        auto layout = new QHBoxLayout(container);
        layout->setContentsMargins({});
        auto label = new QLabel(i18n("Status:"), container);
        label->setObjectName(QStringLiteral("label"));
        layout->addWidget(label);

        mStatusComboBox = new StatusCombobox(container);
        mStatusComboBox->setObjectName(QStringLiteral("mStatusComboBox"));
        layout->addWidget(mStatusComboBox);
        connect(mStatusComboBox, &StatusCombobox::currentIndexChanged, this, &RuqolaMainWindow::slotStatusChanged);

        mStatus = action;
        connect(mStatus, &QAction::triggered, mStatusComboBox, &QComboBox::showPopup);
        ac->addAction(QStringLiteral("status"), mStatus);
    }

    {
        QList<QAction *> listActions;
        auto act = new QAction(i18n("Previous Selected Channel"), this);
        ac->setDefaultShortcut(act, QKeySequence(Qt::CTRL | Qt::Key_Tab));
        ac->addAction(QStringLiteral("previous_channel"), act);
        listActions.append(act);

        connect(act, &QAction::triggered, this, &RuqolaMainWindow::undoSwitchChannel);

        act = new QAction(i18n("Next Selected Channel"), this);
        ac->addAction(QStringLiteral("next_channel"), act);
        ac->setDefaultShortcut(act, QKeySequence(Qt::SHIFT | Qt::Key_Tab | Qt::CTRL));
        connect(act, &QAction::triggered, this, &RuqolaMainWindow::redoSwitchChannel);
        listActions.append(act);

        mSwitchChannelTreeManager->addActions(listActions);
    }

    if (menuBar()) {
        mHamburgerMenu = KStandardAction::hamburgerMenu(nullptr, nullptr, actionCollection());
        mHamburgerMenu->setShowMenuBarAction(mShowMenuBarAction);
        mHamburgerMenu->setMenuBar(menuBar());
        connect(mHamburgerMenu, &KHamburgerMenu::aboutToShowMenu, this, [this]() {
            updateHamburgerMenu();
            // Immediately disconnect. We only need to run this once, but on demand.
            // NOTE: The nullptr at the end disconnects all connections between
            // q and mHamburgerMenu's aboutToShowMenu signal.
            disconnect(mHamburgerMenu, &KHamburgerMenu::aboutToShowMenu, this, nullptr);
        });
    }

    auto manager = new KColorSchemeManager(this);
    ac->addAction(QStringLiteral("colorscheme_menu"), manager->createSchemeSelectionMenu(this));

    mShowFullScreenAction = KStandardAction::fullScreen(nullptr, nullptr, this, ac);
    ac->setDefaultShortcut(mShowFullScreenAction, Qt::Key_F11);
    connect(mShowFullScreenAction, &QAction::toggled, this, &RuqolaMainWindow::slotFullScreen);

    mShowNotifyHistory = new QAction(QIcon::fromTheme(QStringLiteral("notifications")), i18n("Show Notify History"), this);
    connect(mShowNotifyHistory, &QAction::triggered, this, &RuqolaMainWindow::slotOpenNotificationHistory);
    ac->addAction(QStringLiteral("show_notify_history"), mShowNotifyHistory);

    mShowRocketChatServerInfo = new QAction(i18n("Show RocketChat Information"), this);
    connect(mShowRocketChatServerInfo, &QAction::triggered, this, &RuqolaMainWindow::slotRocketChatInformation);
    ac->addAction(QStringLiteral("show_rocketchat_information"), mShowRocketChatServerInfo);

    mRoomAvatar = new QAction(i18n("Room Avatar"), this);
    mRoomAvatar->setCheckable(true);
    connect(mRoomAvatar, &QAction::triggered, this, &RuqolaMainWindow::slotShowRoomAvatar);
    ac->addAction(QStringLiteral("room_avatar"), mRoomAvatar);
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
        mAccountManager->addAccount(info);
    }
    delete dlg;
}

void RuqolaMainWindow::slotServerInfo()
{
    ServerInfoDialog dlg(this);
    dlg.setServerConfigInfo(mCurrentRocketChatAccount->serverConfigInfo());
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
                job->setUiDelegate(new KIO::JobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, this));
                job->setDeleteTemporaryFile(true);
                job->start();
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
    mCreateNewChannel->setEnabled(!loginPageActivated);
    mCreateDirectMessages->setEnabled(!loginPageActivated);
    mLogout->setEnabled(!loginPageActivated);
    mClearAlerts->setEnabled(!loginPageActivated);
    mMyAccount->setEnabled(!loginPageActivated);
    mStatus->setEnabled(!loginPageActivated);
    mCreateDiscussion->setEnabled(!loginPageActivated);
    mCreateTeam->setEnabled(!loginPageActivated);
    mDirectory->setEnabled(!loginPageActivated);
    mNextUnreadChannel->setEnabled(!loginPageActivated);
    mShowLog->setEnabled(!loginPageActivated);
    mShowRocketChatServerInfo->setVisible(!loginPageActivated && hasBannerInfo());
    mRoomAvatar->setEnabled(!loginPageActivated);
}

void RuqolaMainWindow::slotConfigureNotifications()
{
    KNotifyConfigWidget::configure(this);
}

void RuqolaMainWindow::slotRegisterNewUser()
{
    RegisterUserDialog dlg(mCurrentRocketChatAccount, this);
    dlg.exec();
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
        trayMenu->addAction(actionCollection()->action(QLatin1String(KStandardAction::name(KStandardAction::Preferences))));
        trayMenu->addAction(actionCollection()->action(QLatin1String(KStandardAction::name(KStandardAction::ConfigureNotifications))));
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

void RuqolaMainWindow::slotUpdateCustomUserStatus()
{
    mStatusProxyModel->sort(0);
}

void RuqolaMainWindow::slotMessageUrlNotFound(const QString &str)
{
    KMessageBox::information(this, str, i18n("Message"));
}

void RuqolaMainWindow::slotHistorySwitchChannel(const QString &identifier)
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
                const QString accel = mShowMenuBarAction->shortcut().toString();
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
    auto menu = new QMenu(this);
    menu->addAction(actionCollection()->action(QStringLiteral("add_server")));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(QStringLiteral("configure_my_account")));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(QStringLiteral("directory")));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(QStringLiteral("logout")));
    menu->addSeparator();
    menu->addAction(actionCollection()->action(QLatin1String(KStandardAction::name(KStandardAction::Quit))));
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

void RuqolaMainWindow::slotShowNotifyMessage(const QString &accountName, const QString &messageId, const QString &roomId)
{
    Ruqola::self()->setCurrentAccount(accountName);
    Q_EMIT mCurrentRocketChatAccount->selectRoomByRoomIdRequested(roomId);
    // TODO use messageId too
}

void RuqolaMainWindow::slotOpenNotificationHistory()
{
    mNotificationToolButton->hide();
    NotificationHistoryDialog dlg(this);
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
