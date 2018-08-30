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

#include "rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "ruqolaserverconfig.h"
#include "ruqola_debug.h"

RocketChatAccountModel::RocketChatAccountModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

RocketChatAccountModel::~RocketChatAccountModel()
{
    qDeleteAll(mRocketChatAccount);
}

void RocketChatAccountModel::clear()
{
    if (!mRocketChatAccount.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        qDeleteAll(mRocketChatAccount);
        mRocketChatAccount.clear();
        endRemoveRows();
    }
}

void RocketChatAccountModel::setAccounts(const QVector<RocketChatAccount *> &accounts)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mRocketChatAccount.count() - 1);
        mRocketChatAccount.clear();
        endRemoveRows();
    }
    if (!accounts.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, accounts.count() - 1);
        mRocketChatAccount = accounts;
        endInsertRows();
    }
}

RocketChatAccount *RocketChatAccountModel::account(int index) const
{
    if (mRocketChatAccount.isEmpty() || (index > mRocketChatAccount.count() - 1)) {
        qCWarning(RUQOLA_LOG) << " Empty account";
        return nullptr;
    }
    return mRocketChatAccount.at(index);
}

int RocketChatAccountModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mRocketChatAccount.count();
}

QVariant RocketChatAccountModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        qCWarning(RUQOLA_LOG) << "ERROR: invalid index";
        return {};
    }
    const int idx = index.row();
    switch (role) {
    case Name:
        return mRocketChatAccount.at(idx)->accountName();
    case SiteUrl:
        return mRocketChatAccount.at(idx)->ruqolaServerConfig()->siteUrl();
    }
    //Add icon ???
    return {};
}

void RocketChatAccountModel::insertAccount(RocketChatAccount *account)
{
    //Verify that we have it ?
    const int accountCount = mRocketChatAccount.count();
    beginInsertRows(QModelIndex(), accountCount, accountCount);
    mRocketChatAccount.append(account);
    endInsertRows();
}

void RocketChatAccountModel::removeAccount(const QString &name)
{
    qDebug() << " void RocketChatAccountModel::removeAccount(const QString &name)"<<name;
    //Search account.
    for (int i = 0; i < mRocketChatAccount.count(); ++i) {
        if (mRocketChatAccount.at(i)->accountName() == name) {
            beginRemoveRows(QModelIndex(), i, i);
            RocketChatAccount *account = mRocketChatAccount.takeAt(i);
            account->removeSettings();
            delete account;
            endRemoveRows();
            break;
        }
    }
}

QHash<int, QByteArray> RocketChatAccountModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = QByteArrayLiteral("name");
    roles[SiteUrl] = QByteArrayLiteral("siteurl");
    return roles;
}
