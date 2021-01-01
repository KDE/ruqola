/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QObject>
#include "libruqolacore_export.h"
#include "utils.h"
class RocketChatAccountModel;
class RocketChatAccount;
class RocketChatAccountFilterProxyModel;

class LIBRUQOLACORE_EXPORT AccountManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentAccount READ currentAccount WRITE setCurrentAccount NOTIFY currentAccountChanged)
    Q_PROPERTY(RocketChatAccount *account READ account NOTIFY currentAccountChanged)
    Q_PROPERTY(RocketChatAccountModel *rocketChatAccountModel READ rocketChatAccountModel CONSTANT)
    Q_PROPERTY(RocketChatAccountFilterProxyModel *rocketChatAccountProxyModel READ rocketChatAccountProxyModel CONSTANT)
public:
    struct LIBRUQOLACORE_EXPORT AccountManagerInfo {
        QString displayName;
        QString accountName;
        QString userName;
        QString serverUrl;
        bool enabled = true;
    };

    explicit AccountManager(QObject *parent = nullptr);
    ~AccountManager() override;

    void removeAccount(const QString &accountName);
    void addAccount(const AccountManagerInfo &info);

    RocketChatAccount *account() const;
    RocketChatAccountModel *rocketChatAccountModel() const;

    RocketChatAccountFilterProxyModel *rocketChatAccountProxyModel() const;

    void addAccount(RocketChatAccount *account);

    void setCurrentAccount(const QString &accountName);
    Q_REQUIRED_RESULT QString currentAccount() const;
    void selectAccount(const QString &accountName);

    void modifyAccount(const AccountManagerInfo &info);
    Q_REQUIRED_RESULT QStringList accountsName() const;

    Q_REQUIRED_RESULT int accountNumber() const;
Q_SIGNALS:
    void logoutAccountDone(const QString &accountName);
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    void currentAccountChanged();
    void roomNeedAttention();

private:
    Q_DISABLE_COPY(AccountManager)

    void loadAccount();
    void connectToAccount(RocketChatAccount *account);
    void slotSwitchToAccountAndRoomName(const QString &accountName, const QString &roomName, const QString &channelType);
    RocketChatAccount *mCurrentAccount = nullptr;
    RocketChatAccountModel *const mRocketChatAccountModel;
    RocketChatAccountFilterProxyModel *const mRocketChatAccountProxyModel;
};

#endif // ACCOUNTMANAGER_H
