/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include "channelgroupbasejob.h"
#include "libruqolawidgets_export.h"
#include <KXmlGuiWindow>
#include <QPointer>
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
class LIBRUQOLAWIDGETS_EXPORT RuqolaMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit RuqolaMainWindow(QWidget *parent = nullptr);
    ~RuqolaMainWindow() override;

protected:
    Q_REQUIRED_RESULT bool queryClose() override;

private:
    void slotRegisterUserSuccessed();
    void setupActions();
    void readConfig();
    void setupStatusBar();
    void slotAddServer();
    void slotServerInfo();
    void slotConfigure();
    void slotLogout();
    void slotSearchChannel();
    void slotSelectNextUnreadChannel();
    void slotCreateNewChannel();
    void slotTypingNotificationChanged(const QString &roomId, const QString &notificationStr);
    void slotClearNotification();
    void slotAccountChanged();
    void slotUnreadOnTop(bool checked);
    void slotShowLog();
    void updateActions();
    void slotMissingChannelPassword(const RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo &channelInfo);
    void slotClearAccountAlerts();
    void slotLoginPageActivated(bool loginPageActivated);
    void slotConfigureNotifications();
    void slotRaiseWindow();
    void slotRegisterNewUser();
    void slotAdministrator();
    void slotConfigureMyAccount();
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    void logout(const QString &accountName);
    void slotRoomNeedAttention();
    void slotCreateDirectMessages();
    void slotClose();
    void slotStatusChanged();
    void createSystemTray();
    void slotUpdateCustomUserStatus();
    void slotCreateDiscussion();
    void slotCreateTeam();
    void slotPermissionChanged();
    void slotDirectory();
    bool mReallyClose{false};

    RuqolaCentralWidget *const mMainWidget;
    QAction *mServerInfo = nullptr;
    QAction *mLogout = nullptr;
    QAction *mSearchChannel = nullptr;
    QAction *mNextUnreadChannel = nullptr;
    QAction *mCreateNewChannel = nullptr;
    QAction *mCreateDirectMessages = nullptr;
    QAction *mUnreadOnTop = nullptr;
    QAction *mShowLog = nullptr;
    QAction *mClearAlerts = nullptr;
    QAction *mRegisterNewUser = nullptr;
    QAction *mMyAccount = nullptr;
    QAction *mAdministrator = nullptr;
    QAction *mCreateDiscussion = nullptr;
    QAction *mCreateTeam = nullptr;
    QAction *mDirectory = nullptr;
    QWidgetAction *mStatus = nullptr;
    ServerMenu *mServerMenu = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QLabel *mStatusBarTypingMessage = nullptr;
    AccountsOverviewWidget *mAccountOverviewWidget = nullptr;
    Notification *mNotification = nullptr;
    StatusCombobox *mStatusComboBox = nullptr;
    StatusModelFilterProxyModel *mStatusProxyModel = nullptr;
};

