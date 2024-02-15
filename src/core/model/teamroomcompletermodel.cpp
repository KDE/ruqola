/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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

void TeamRoomCompleterModel::setRooms(const QVector<TeamRoomCompleter> &teams)
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
    Q_UNUSED(parent)
    return mRooms.count();
}

QVariant TeamRoomCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRooms.count()) {
        return {};
    }
    const TeamRoomCompleter &room = mRooms.at(index.row());
    switch (role) {
    case TeamName:
        return room.name();
    case TeamId:
        return room.identifier();
    }

    return {};
}

#include "moc_teamroomcompletermodel.cpp"
