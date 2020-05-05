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

#include "accountmanager.h"
#include "rocketchataccount.h"
#include "managerdatapaths.h"
#include "ruqola_debug.h"
#include "notifierjob.h"
#include <QDir>
#include <QDirIterator>
#include <QSettings>

#include "model/rocketchataccountmodel.h"
#include "model/rocketchataccountfilterproxymodel.h"

AccountManager::AccountManager(QObject *parent)
    : QObject(parent)
{
    mRocketChatAccountModel = new RocketChatAccountModel(this);
    mRocketChatAccountProxyModel = new RocketChatAccountFilterProxyModel(this);
    mRocketChatAccountProxyModel->setSourceModel(mRocketChatAccountModel);
    loadAccount();
}

AccountManager::~AccountManager()
{
}

void AccountManager::connectToAccount(RocketChatAccount *account)
{
    connect(account, &RocketChatAccount::notification, this, [this, account](const Utils::NotificationInfo &info) {
        NotifierJob *job = new NotifierJob;
        job->setInfo(info);
        job->setAccountName(account->accountName());
        connect(job, &NotifierJob::switchToAccountAndRoomName, this, &AccountManager::slotSwitchToAccountAndRoomName);
        job->start();
    });
    connect(account, &RocketChatAccount::updateNotification, this, &AccountManager::updateNotification);
    connect(account, &RocketChatAccount::logoutDone, this, &AccountManager::logoutAccountDone);
}

void AccountManager::slotSwitchToAccountAndRoomName(const QString &accountName, const QString &roomName, const QString &channelType)
{
    setCurrentAccount(accountName);
    QString linkRoom;
    if (channelType == QLatin1Char('c')) {
        linkRoom = QStringLiteral("ruqola:/room/%1").arg(roomName);
    } else {
        linkRoom = QStringLiteral("ruqola:/user/%1").arg(roomName);
    }
    Q_EMIT mCurrentAccount->openLinkRequested(linkRoom);
}

void AccountManager::loadAccount()
{
    qCDebug(RUQOLA_LOG) << " void AccountManager::loadAccount()"<<ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString());
    QDirIterator it(ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString()), QStringList() << QStringLiteral(
                        "ruqola.conf"), QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QVector<RocketChatAccount *> lstAccounts;
    while (it.hasNext()) {
        const QString val = it.next();
        qCDebug(RUQOLA_LOG) << "Account found list.at(i)" << val;
        auto *account = new RocketChatAccount(val);
        connectToAccount(account);
        lstAccounts.append(account);
    }

    //New account => empty list.
    if (lstAccounts.isEmpty()) {
        qCDebug(RUQOLA_LOG) << "Empty list. Create a default rocketchataccount";
        RocketChatAccount *account = new RocketChatAccount();
        if (account->accountEnabled()) {
            connectToAccount(account);
        }
        lstAccounts.append(account);
    }

    mRocketChatAccountModel->setAccounts(lstAccounts);

    QSettings settings;
    const QString previousAccount = settings.value(QStringLiteral("currentAccount"), QString()).toString();
    if (previousAccount.isEmpty()) {
        //Use first one
        mCurrentAccount = mRocketChatAccountModel->account(0);
    } else {
        selectAccount(previousAccount);
        if (!mCurrentAccount) {
            //Use first one
            mCurrentAccount = mRocketChatAccountModel->account(0);
        }
    }
}

RocketChatAccountFilterProxyModel *AccountManager::rocketChatAccountProxyModel() const
{
    return mRocketChatAccountProxyModel;
}

RocketChatAccount *AccountManager::account() const
{
    return mCurrentAccount;
}

void AccountManager::addAccount(const QString &accountName, const QString &username, const QString &url, bool enabled)
{
    //TODO verify if account exist or not ?
    RocketChatAccount *account = new RocketChatAccount();
    account->setAccountName(accountName);
    account->setUserName(username);
    account->setServerUrl(url);
    account->setAccountEnabled(enabled);
    if (enabled) {
        connectToAccount(account);
    }
    addAccount(account);
}

void AccountManager::modifyAccount(const QString &accountName, const QString &username, const QString &url, bool enabled)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(accountName);
    if (account) {
        account->setUserName(username);
        account->setServerUrl(url);
        account->setAccountEnabled(enabled);
        if (!enabled) {
            //TODO fixme
            //disconnect(account, &RocketChatAccount::notification, this, &AccountManager::notification);
            disconnect(account, &RocketChatAccount::updateNotification, this, &AccountManager::updateNotification);
            disconnect(account, &RocketChatAccount::logoutDone, this, &AccountManager::logoutAccountDone);
        }
    }
}

QStringList AccountManager::accountsName() const
{
    return mRocketChatAccountModel->accountsName();
}

void AccountManager::addAccount(RocketChatAccount *account)
{
    mRocketChatAccountModel->insertAccount(account);
}

void AccountManager::selectAccount(const QString &accountName)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(accountName);
    if (account) {
        mCurrentAccount = account;
    } else {
        qCWarning(RUQOLA_LOG) << "AccountName " << accountName << " is not found on system.";
    }
}

void AccountManager::setCurrentAccount(const QString &accountName)
{
    RocketChatAccount *account = mRocketChatAccountModel->account(accountName);
    if (account) {
        if (mCurrentAccount != account) {
            QSettings settings;
            settings.setValue(QStringLiteral("currentAccount"), accountName);
            settings.sync();
            mCurrentAccount = account;
            Q_EMIT currentAccountChanged();
        }
    } else {
        qCWarning(RUQOLA_LOG) << "AccountName " << accountName << " is not found on system. Fallback to default one.";
    }
}

QString AccountManager::currentAccount() const
{
    return mCurrentAccount ? mCurrentAccount->accountName() : QString();
}

void AccountManager::removeAccount(const QString &accountName)
{
    mRocketChatAccountModel->removeAccount(accountName);
    if (mRocketChatAccountModel->accountNumber() > 0) {
        mCurrentAccount = mRocketChatAccountModel->account(0);
    } else {
        //TODO create new dummy account !
    }
    Q_EMIT currentAccountChanged();
}

RocketChatAccountModel *AccountManager::rocketChatAccountModel() const
{
    return mRocketChatAccountModel;
}
