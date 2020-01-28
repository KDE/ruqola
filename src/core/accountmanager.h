/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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
#include "libruqola_private_export.h"
class RocketChatAccountModel;
class RocketChatAccount;
class RocketChatAccountFilterProxyModel;

class LIBRUQOLACORE_TESTS_EXPORT AccountManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentAccount READ currentAccount WRITE setCurrentAccount NOTIFY currentAccountChanged)
    Q_PROPERTY(RocketChatAccount *account READ account NOTIFY currentAccountChanged)
    Q_PROPERTY(RocketChatAccountModel *rocketChatAccountModel READ rocketChatAccountModel CONSTANT)
    Q_PROPERTY(RocketChatAccountFilterProxyModel *rocketChatAccountProxyModel READ rocketChatAccountProxyModel CONSTANT)
public:
    explicit AccountManager(QObject *parent = nullptr);
    ~AccountManager();

    Q_INVOKABLE void removeAccount(const QString &accountName);
    Q_INVOKABLE void addAccount(const QString &accountName, const QString &username, const QString &url);

    RocketChatAccount *account() const;
    RocketChatAccountModel *rocketChatAccountModel() const;

    RocketChatAccountFilterProxyModel *rocketChatAccountProxyModel() const;

    void addAccount(RocketChatAccount *account);

    void setCurrentAccount(const QString &accountName);
    Q_REQUIRED_RESULT QString currentAccount() const;
    void selectAccount(const QString &accountName);

Q_SIGNALS:
    void logoutAccountDone(const QString &accountName);
    void updateNotification(bool hasAlert, int nbUnread, const QString &accountName);
    void notification(const QString &title, const QString &message, const QPixmap &pixmap);
    void currentAccountChanged();

private:
    Q_DISABLE_COPY(AccountManager)

    void loadAccount();
    RocketChatAccount *mCurrentAccount = nullptr;
    RocketChatAccountModel *mRocketChatAccountModel = nullptr;
    RocketChatAccountFilterProxyModel *mRocketChatAccountProxyModel = nullptr;
};

#endif // ACCOUNTMANAGER_H
