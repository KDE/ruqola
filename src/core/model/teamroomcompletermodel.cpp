/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamroomcompletermodel.h"
#include <QModelIndex>

TeamRoomCompleterModel::TeamRoomCompleterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

TeamRoomCompleterModel::~TeamRoomCompleterModel() = default;

void TeamRoomCompleterModel::clear()
{
    if (!mRooms.isEmpty()) {
        beginResetModel();
        mRooms.clear();
        endResetModel();
    }
}

void TeamRoomCompleterModel::setRooms(const QList<TeamRoomCompleter> &teams)
{
    clear();
    if (!teams.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, teams.count() - 1);
        mRooms = teams;
        endInsertRows();
    }
}

int TeamRoomCompleterModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }
    return mRooms.count();
}

QVariant TeamRoomCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRooms.count()) {
        return {};
    }
    const TeamRoomCompleter &room = mRooms.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case TeamName:
        return room.name();
    case TeamId:
        return room.identifier();
    case AvatarInfo:
        return QVariant::fromValue(avatarInfo(room));
    }

    return {};
}

Utils::AvatarInfo TeamRoomCompleterModel::avatarInfo(const TeamRoomCompleter &room) const
{
    Utils::AvatarInfo info;
    info.avatarType = Utils::AvatarType::Room;
    info.identifier = QString::fromLatin1(room.identifier());
    return info;
}
#include "moc_teamroomcompletermodel.cpp"
