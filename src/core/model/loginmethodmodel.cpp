/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loginmethodmodel.h"
#include <QModelIndex>

LoginMethodModel::LoginMethodModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

LoginMethodModel::~LoginMethodModel() = default;

int LoginMethodModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mAuthentications.count();
}

void LoginMethodModel::clear()
{
    if (!mAuthentications.isEmpty()) {
        beginResetModel();
        mAuthentications.clear();
        endResetModel();
    }
}

AuthenticationManager::OauthType LoginMethodModel::loginType(int index)
{
    return mAuthentications.at(index).oauthType();
}

int LoginMethodModel::currentLoginMethod() const
{
    return mCurrentLoginMethod;
}

void LoginMethodModel::setCurrentLoginMethod(int currentLoginMethod)
{
    if (mCurrentLoginMethod != currentLoginMethod) {
        mCurrentLoginMethod = currentLoginMethod;
        Q_EMIT currentLoginMethodChanged();
    }
}

QVector<AuthenticationInfo> LoginMethodModel::authentications() const
{
    return mAuthentications;
}

QVariant LoginMethodModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mAuthentications.count()) {
        return {};
    }

    const AuthenticationInfo info = mAuthentications.at(index.row());
    switch (role) {
    case NameRole:
        return info.name();
    case IconNameRole:
        return info.iconName();
    case TypeRole:
        return info.oauthType();
    }

    return {};
}

void LoginMethodModel::setAuthenticationInfos(const QVector<AuthenticationInfo> &infos)
{
    clear();
    if (!infos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, infos.count() - 1);
        mAuthentications = infos;
        endInsertRows();
    }
}

#include "moc_loginmethodmodel.cpp"
