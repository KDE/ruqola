/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "utils.h"

#include <QObject>

class LIBRUQOLACORE_EXPORT AccountManager : public QObject
{
    Q_OBJECT
public:
    struct LIBRUQOLACORE_EXPORT AccountManagerInfo {
        QString displayName;
        QString accountName;
        QString userName;
        QString serverUrl;
        QString password;
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

