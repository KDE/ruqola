/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include "ruqolaserverconfig.h"

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
    setAccounts({});
}

void RocketChatAccountModel::setAccounts(const QVector<RocketChatAccount *> &accounts)
{
    if (accounts == mRocketChatAccount) {
        return;
    }

    beginResetModel();
    qDeleteAll(mRocketChatAccount);
    mRocketChatAccount = accounts;
    endResetModel();

    Q_EMIT accountNumberChanged();
}

RocketChatAccount *RocketChatAccountModel::accountFromServerUrl(const QString &serverUrl) const
{
    if (mRocketChatAccount.isEmpty()) {
        qCWarning(RUQOLA_LOG) << " Empty account";
        return nullptr;
    }
    for (int i = 0, total = mRocketChatAccount.count(); i < total; ++i) {
        RocketChatAccount *model = mRocketChatAccount.at(i);
        if (model->serverUrl().contains(serverUrl)) {
            return model;
        }
    }
    return nullptr;
}

RocketChatAccount *RocketChatAccountModel::account(const QString &accountName) const
{
    if (mRocketChatAccount.isEmpty()) {
        qCWarning(RUQOLA_LOG) << " Empty account";
        return nullptr;
    }
    for (int i = 0, total = mRocketChatAccount.count(); i < total; ++i) {
        RocketChatAccount *model = mRocketChatAccount.at(i);
        if (model->accountName() == accountName) {
            return model;
        }
    }
    return nullptr;
}

int RocketChatAccountModel::accountNumber() const
{
    return mRocketChatAccount.count();
}

QStringList RocketChatAccountModel::accountsName() const
{
    QStringList accounts;
    for (int i = 0, total = mRocketChatAccount.count(); i < total; ++i) {
        RocketChatAccount *model = mRocketChatAccount.at(i);
        accounts << model->accountName();
    }
    return accounts;
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
    RocketChatAccount *account = mRocketChatAccount.at(idx);
    switch (role) {
    case Qt::DisplayRole:
    case Name:
        return account->displayName();
    case SiteUrl:
        return account->ruqolaServerConfig()->siteUrl();
    case UserName:
        return account->userName();
    case Account:
        return QVariant::fromValue(account);
    }
    // Add icon ???
    return {};
}

void RocketChatAccountModel::insertAccount(RocketChatAccount *account)
{
    // Verify that we have it ?
    const int accountCount = mRocketChatAccount.count();
    beginInsertRows(QModelIndex(), accountCount, accountCount);
    mRocketChatAccount.append(account);
    endInsertRows();
    Q_EMIT accountNumberChanged();
}

void RocketChatAccountModel::removeAccount(const QString &name)
{
    // qDebug() << " void RocketChatAccountModel::removeAccount(const QString &name)"<<name;
    // Search account.
    for (int i = 0, total = mRocketChatAccount.count(); i < total; ++i) {
        if (mRocketChatAccount.at(i)->accountName() == name) {
            beginRemoveRows(QModelIndex(), i, i);
            RocketChatAccount *account = mRocketChatAccount.takeAt(i);
            account->removeSettings();
            delete account;
            endRemoveRows();
            Q_EMIT accountNumberChanged();
            break;
        }
    }
}
