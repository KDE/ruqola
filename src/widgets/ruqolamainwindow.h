/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channelgroupbasejob.h"
#include "libruqolawidgets_export.h"
#include <KXmlGuiWindow>
#include <QPointer>
#include <config-ruqola.h>
class StatusCombobox;
class RuqolaCentralWidget;
class QAction;
class QWidgetAction;
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
class AccountManager;
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
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTypingNotificationChanged(const QString &roomId, const QString &notificationStr);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotClearNotification();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAccountChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUnreadOnTop(bool checked);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRoomListSortByLastMessage();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRoomListSortAlphabetically();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowLog();
    LIBRUQOLAWIDGETS_NO_EXPORT void updateActions();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotMissingChannelPassword(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotClearAccountAlerts();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotLoginPageActivated(bool loginPageActivated);
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
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDirectory();
    LIBRUQOLAWIDGETS_NO_EXPORT void showNextView();
    LIBRUQOLAWIDGETS_NO_EXPORT void showPreviousView();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotMessageUrlNotFound(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotHistorySwitchChannel(const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void undoSwitchChannel();
    LIBRUQOLAWIDGETS_NO_EXPORT void redoSwitchChannel();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotToggleMenubar(bool dontShowWarning);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateHamburgerMenu();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFullScreen(bool t);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenNotificationHistory();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotNewNotification();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowNotifyMessage(const QString &accountName, const QString &messageId, const QString &roomId);
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
    QAction *mCreateDiscussion = nullptr;
    QAction *mCreateTeam = nullptr;
    QAction *mDirectory = nullptr;
    QAction *mNextTab = nullptr;
    QAction *mPreviewTab = nullptr;
    QWidgetAction *mStatus = nullptr;
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
    KActionMenu *mAdministratorMenu = nullptr;
    AccountManager *mAccountManager = nullptr;
    QAction *mShowRocketChatServerInfo = nullptr;
    QAction *mRoomAvatar = nullptr;
    QAction *mRoomFavorite = nullptr;
    QAction *mShowDatabaseMessages = nullptr;
    QAction *mMenuDebug = nullptr;
    QAction *mShowPermissions = nullptr;

    QAction *mImportAccountsAction = nullptr;
    QAction *mExportAccountsAction = nullptr;
};
