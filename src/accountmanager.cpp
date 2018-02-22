/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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
#include <QDir>
#include <QDirIterator>
#include <QFileInfoList>

#include "model/rocketchataccountmodel.h"

AccountManager::AccountManager(QObject *parent)
    : QObject(parent)
{
    mRocketChatAccountModel = new RocketChatAccountModel(this);
    loadAccount();
}

AccountManager::~AccountManager()
{
}

void AccountManager::loadAccount()
{
    qCDebug(RUQOLA_LOG) << " void AccountManager::loadAccount()"<<ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString());
    QDirIterator it(ManagerDataPaths::self()->path(ManagerDataPaths::Config, QString()), QStringList() << QStringLiteral(
                        "ruqola.conf"), QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QVector<RocketChatAccount *> lstAccounts;
    while (it.hasNext()) {
        QString val = it.next();
        qCDebug(RUQOLA_LOG) << "Account found list.at(i)" << val;
        RocketChatAccount *account = new RocketChatAccount(val);
        connect(account, &RocketChatAccount::notification, this, &AccountManager::notification);
        connect(account, &RocketChatAccount::updateNotification, this, &AccountManager::updateNotification);
        connect(account, &RocketChatAccount::logoutDone, this, &AccountManager::logoutAccountDone);
        lstAccounts.append(account);
    }

    //New account => empty list.
    if(lstAccounts.isEmpty()) {
        qCDebug(RUQOLA_LOG) << "Empty list. Create a default rocketchataccout";
        RocketChatAccount *account = new RocketChatAccount();
        connect(account, &RocketChatAccount::notification, this, &AccountManager::notification);
        connect(account, &RocketChatAccount::updateNotification, this, &AccountManager::updateNotification);
        connect(account, &RocketChatAccount::logoutDone, this, &AccountManager::logoutAccountDone);
        lstAccounts.append(account);
    }

    mRocketChatAccountModel->insertAccounts(lstAccounts);
}

RocketChatAccount *AccountManager::firstAccount() const
{
    //Temporary too
    RocketChatAccount *t = mRocketChatAccountModel->account(0);
    qDebug() << "RocketChatAccount *AccountManager::firstAccount() const "<<t;
    return t;
}

void AccountManager::addAccount(RocketChatAccount *account)
{
    mRocketChatAccountModel->insertAccount(account);
}

void AccountManager::removeAccount(const QString &accountName)
{
    mRocketChatAccountModel->removeAccount(accountName);
}

RocketChatAccountModel *AccountManager::rocketChatAccountModel() const
{
    return mRocketChatAccountModel;
}
