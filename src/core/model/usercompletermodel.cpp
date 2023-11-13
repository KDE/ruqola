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
        beginResetModel();
        mUsers.clear();
        endResetModel();
    }
}

void UserCompleterModel::insertUsers(const QVector<User> &users)
{
    clear();
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
    case DisplayName:
        return displayUserName(user);
    case UserName:
        return user.userName();
    case UserId:
        return user.userId();
    case UserIconStatus:
    case Qt::DecorationRole:
        return QIcon::fromTheme(user.iconFromStatus());
    case AvatarInfo:
        return QVariant::fromValue(avatarInfo(user));
    }

    return {};
}

Utils::AvatarInfo UserCompleterModel::avatarInfo(const User &user) const
{
    Utils::AvatarInfo info;
    info.avatarType = Utils::AvatarType::User;
    info.identifier = user.userName();
    return info;
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

#include "moc_usercompletermodel.cpp"
