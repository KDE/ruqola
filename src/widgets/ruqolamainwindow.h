/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "accountmanager.h"
#include "applicationssettingsdialog/applicationssettingssearchwidget.h"
#include "channelgroupbasejob.h"
#include "config-ruqola.h"
#include "libruqolawidgets_export.h"
#include <KXmlGuiWindow>
#include <QPointer>
class StatusCombobox;
class RuqolaCentralWidget;
class QAction;
class ServerMenu;
class RocketChatAccount;
class QLabel;
class AccountsOverviewWidget;
class Notification;
class StatusModelFilterProxyModel;
class QCommandLineParser;
class SwitchChannelTreeViewManager;
class KToggleAction;
class KHamburgerMenu;
class KToggleFullScreenAction;
class QToolButton;
class KActionMenu;
class MessageStyleLayoutMenu;
class ChangeFontSizeMenu;
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
namespace TextAddonsWidgets
{
class VerifyNewVersionWidget;
}
#endif
class LIBRUQOLAWIDGETS_EXPORT RuqolaMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit RuqolaMainWindow(QWidget *parent = nullptr);
    ~RuqolaMainWindow() override;

    void slotActivateRequested(const QStringList &arguments, const QString &workingDirectory);

    void parseCommandLine(QCommandLineParser *parser);

protected:
    [[nodiscard]] bool queryClose() override;

private:
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool canCreateChannels() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool canCreateDirectMessages() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool canCreateTeams() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRegisterUserSuccessed();
    LIBRUQOLAWIDGETS_NO_EXPORT void setupActions();
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void setupStatusBar();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddServer();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotServerInfo();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotConfigure();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLogout();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSelectNextUnreadChannel();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCreateNewChannel();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTypingNotificationChanged(const QByteArray &roomId, const QString &notificationStr);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotClearNotification();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAccountChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUnreadOnTop(bool checked);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRoomListSortByLastMessage();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRoomListSortAlphabetically();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowLog();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateActions();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotMissingChannelPassword(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotClearAccountAlerts();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDisableActions(bool loginPageActivated);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotConfigureNotifications();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRaiseWindow();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRegisterNewUser();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAdministrator();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotConfigureMyAccount();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    LIBRUQOLAWIDGETS_NO_EXPORT void logout(const QString &accountName);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRoomNeedAttention();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCreateDirectMessages();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotClose();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotStatusChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateStatusMenu();
    LIBRUQOLAWIDGETS_NO_EXPORT void createSystemTray();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateCustomUserStatus();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCreateDiscussion();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCreateTeam();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPermissionChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void showNextView();
    LIBRUQOLAWIDGETS_NO_EXPORT void showPreviousView();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotHistorySwitchChannel(const QByteArray &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void undoSwitchChannel();
    LIBRUQOLAWIDGETS_NO_EXPORT void redoSwitchChannel();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotToggleMenubar(bool dontShowWarning);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateHamburgerMenu();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFullScreen(bool t);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenNotificationHistory();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotNewNotification();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowNotifyMessage(const QString &accountName, const QByteArray &messageId, const QByteArray &roomId);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAdministratorServerSettings();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRocketChatInformation();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool hasBannerInfo() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowRoomAvatar(bool checked);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPrivateSettingsChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowFavoriteRoom(bool checked);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowServerInfo();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotWhatsNew();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowDatabaseMessages();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowPermissions();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotImportAccounts();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotExportAccounts();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotMessageStyleChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotApplicationsSettings();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotJoinTeam();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenPrivateChannel();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotJoinRoom();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotApplicationsRequestedSettings();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotApplicationsInstalledSettings();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotClearRoomHistory();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowLogsFile();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowRestApiLogsFile();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowDatabaseLogsFile();
    LIBRUQOLAWIDGETS_NO_EXPORT void applicationSettings(ApplicationsSettingsSearchWidget::Feature feature);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotPrivateApplicationsSettings();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddInviteServer(const AccountManager::AccountManagerInfo &info);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowAppsTranslation();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotWorkOfflineMode();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOfflineModeChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateOfflineAction();

    bool mReallyClose{false};

    RuqolaCentralWidget *const mMainWidget;
    QAction *mServerInfo = nullptr;
    QAction *mLogout = nullptr;
    QAction *mNextUnreadChannel = nullptr;
    QAction *mCreateNewChannel = nullptr;
    QAction *mCreateDirectMessages = nullptr;
    QAction *mUnreadOnTop = nullptr;
    QAction *mRoomListSortByLastMessage = nullptr;
    QAction *mRoomListSortAlphabetically = nullptr;
    QAction *mShowLog = nullptr;
    QAction *mShowServerInfo = nullptr;
    QAction *mClearAlerts = nullptr;
    QAction *mRegisterNewUser = nullptr;
    QAction *mMyAccount = nullptr;
    QAction *mAdministrator = nullptr;
    QAction *mAdministratorServerSettings = nullptr;
    QAction *mApplications = nullptr;
    QAction *mRequestedApplications = nullptr;
    QAction *mInstalledApplications = nullptr;
    QAction *mCreateDiscussion = nullptr;
    QAction *mCreateTeam = nullptr;
    QAction *mJoinRoom = nullptr;
    QAction *mJoinTeam = nullptr;
    QAction *mOpenPrivateConversation = nullptr;
    QAction *mNextTab = nullptr;
    QAction *mPreviewTab = nullptr;
    ServerMenu *mServerMenu = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QLabel *mStatusBarTypingMessage = nullptr;
    AccountsOverviewWidget *mAccountOverviewWidget = nullptr;
    Notification *mNotification = nullptr;
    QMenu *mContextStatusMenu = nullptr;
    StatusCombobox *mStatusComboBox = nullptr;
    StatusModelFilterProxyModel *mStatusProxyModel = nullptr;
    SwitchChannelTreeViewManager *const mSwitchChannelTreeManager;
    KToggleAction *mShowMenuBarAction = nullptr;
    KHamburgerMenu *mHamburgerMenu = nullptr;
    KToggleFullScreenAction *mShowFullScreenAction = nullptr;
    QToolButton *mNotificationToolButton = nullptr;
    QAction *mShowNotifyHistory = nullptr;
    KActionMenu *mAdministrationMenu = nullptr;
    AccountManager *mAccountManager = nullptr;
    QAction *mShowRocketChatServerInfo = nullptr;
    QAction *mRoomAvatar = nullptr;
    QAction *mRoomFavorite = nullptr;
    QAction *mShowDatabaseMessages = nullptr;
    QAction *mMenuDebug = nullptr;
    QAction *mShowPermissions = nullptr;
    QAction *mShowAppsTranslation = nullptr;
    QAction *mShowLogsFile = nullptr;
    QAction *mShowRestApiLogsFile = nullptr;
    QAction *mShowDatabaseLogsFile = nullptr;

    QAction *mImportAccountsAction = nullptr;
    QAction *mExportAccountsAction = nullptr;

    QAction *mRoomListDisplayMedium = nullptr;
    QAction *mRoomListDisplayCondensed = nullptr;
    QAction *mRoomListDisplayExtended = nullptr;
    QWidget *mContainerStatusInfo = nullptr;
    QAction *mClearRoomHistory = nullptr;
    QAction *mPrivateApplications = nullptr;
    QAction *mAddServer = nullptr;
#if ADD_OFFLINE_SUPPORT
    QAction *mOfflineMode = nullptr;
#endif
    MessageStyleLayoutMenu *mMessageStyleAction = nullptr;
    ChangeFontSizeMenu *mChangeFontSizeAction = nullptr;
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    TextAddonsWidgets::VerifyNewVersionWidget *const mVerifyNewVersionWidgetAction;
#endif
};
