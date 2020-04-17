/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef RUQOLAMAINWINDOW_H
#define RUQOLAMAINWINDOW_H

#include <KXmlGuiWindow>
#include <QPointer>
#include "channels/channelbasejob.h"
#include "libruqolawidgets_export.h"
class RuqolaCentralWidget;
class QAction;
class AccountMenu;
class RocketChatAccount;
class QLabel;
class AccountsOverviewWidget;
class LIBRUQOLAWIDGETS_EXPORT RuqolaMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit RuqolaMainWindow(QWidget *parent = nullptr);
    ~RuqolaMainWindow() override;

private:
    void slotSaveAs();
    void setupActions();
    void readConfig();
    void setupStatusBar();
    void slotAddAccount();
    void slotServerInfo();
    void slotConfigure();
    void slotLogout();
    void slotSearchChannel();
    void slotCreateNewChannel();
    void slotTypingNotificationChanged(const QString &roomId, const QString &notificationStr);
    void slotClearNotification();
    void slotAccountChanged();
    void slotUnreadOnTop(bool checked);
    void updateActions();
    void slotMissingChannelPassword(const RocketChatRestApi::ChannelBaseJob::ChannelInfo &channelInfo);
    void changeActionStatus(bool enabled);
    void slotClearAccountAlerts();

    RuqolaCentralWidget *mMainWidget = nullptr;
    QAction *mServerInfo = nullptr;
    QAction *mLogout = nullptr;
    QAction *mSearchChannel = nullptr;
    QAction *mCreateNewChannel = nullptr;
    QAction *mUnreadOnTop = nullptr;
    QAction *mSaveAs = nullptr;
    AccountMenu *mAccountMenu = nullptr;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
    QLabel *mStatusBarTypingMessage = nullptr;
    AccountsOverviewWidget *mAccountOverviewWidget = nullptr;
};

#endif // RUQOLAMAINWINDOW_H
