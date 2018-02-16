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

#include "loginmethodmodel.h"

LoginMethodModel::LoginMethodModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

LoginMethodModel::~LoginMethodModel()
{
}


int LoginMethodModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mAuthentications.count();
}

QVariant LoginMethodModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mAuthentications.count()) {
        return QVariant();
    }

    const AuthenticationInfo info = mAuthentications.at(index.row());
    switch (role) {
    case Name:
        return info.name();
    case IconName:
        return info.iconName();
    case Type:
        return info.oauthType();
    }
    return {};
}

void LoginMethodModel::setAuthenticationInfos(const QVector<AuthenticationInfo> &infos)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mAuthentications.count() - 1);
        mAuthentications.clear();
        endRemoveRows();
    }
    if (!infos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, infos.count() - 1);
        mAuthentications = infos;
        endInsertRows();
    }
}

QHash<int, QByteArray> LoginMethodModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = QByteArrayLiteral("name");
    roles[IconName] = QByteArrayLiteral("iconname");
    roles[Type] = QByteArrayLiteral("type");
    return roles;
}
