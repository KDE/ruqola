/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usercompletermodel.h"
#include <QIcon>
#include <QModelIndex>

UserCompleterModel::UserCompleterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

UserCompleterModel::~UserCompleterModel() = default;

void UserCompleterModel::clear()
{
    if (!mUsers.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        mUsers.clear();
        endRemoveRows();
    }
}

void UserCompleterModel::insertUsers(const QVector<User> &users)
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

int UserCompleterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mUsers.count();
}

QVariant UserCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return {};
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

QString UserCompleterModel::displayUserName(const User &user) const
{
    QString text = user.userName();
    const QString name = user.name();
    if (!name.isEmpty()) {
        text += QLatin1String(" (") + name + QLatin1Char(')');
    }
    return text;
}
