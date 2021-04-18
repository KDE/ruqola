/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "teamroomcompletermodel.h"
#include "ruqola_debug.h"
#include <QIcon>
#include <QModelIndex>

TeamRoomCompleterModel::TeamRoomCompleterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

TeamRoomCompleterModel::~TeamRoomCompleterModel()
{
}

void TeamRoomCompleterModel::clear()
{
    if (!mUsers.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        mUsers.clear();
        endRemoveRows();
    }
}

void TeamRoomCompleterModel::insertUsers(const QVector<User> &users)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mUsers.count() - 1);
        mUsers.clear();
        endRemoveRows();
    }
    if (!users.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, users.count() - 1);
        mUsers = users;
        endInsertRows();
    }
}

int TeamRoomCompleterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mUsers.count();
}

QVariant TeamRoomCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return QVariant();
    }
    const User user = mUsers.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return displayUserName(user);
    case UserName:
        return user.userName();
    case UserId:
        return user.userId();
    case Qt::DecorationRole:
        return QIcon::fromTheme(user.iconFromStatus());
    case UserIconStatus:
        return user.iconFromStatus();
    }

    return {};
}

QString TeamRoomCompleterModel::displayUserName(const User &user) const
{
    QString text = user.userName();
    const QString name = user.name();
    if (!name.isEmpty()) {
        text += QLatin1String(" (") + name + QLatin1Char(')');
    }
    return text;
}
