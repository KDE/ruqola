/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "teamcompletermodel.h"
#include "ruqola_debug.h"
#include <QModelIndex>

TeamCompleterModel::TeamCompleterModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

TeamCompleterModel::~TeamCompleterModel() = default;

void TeamCompleterModel::clear()
{
    if (!mTeams.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        mTeams.clear();
        endRemoveRows();
    }
}

void TeamCompleterModel::insertRooms(const QVector<TeamCompleter> &rooms)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mTeams.count() - 1);
        mTeams.clear();
        endRemoveRows();
    }
    if (!rooms.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, rooms.count() - 1);
        mTeams = rooms;
        endInsertRows();
    }
}

int TeamCompleterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mTeams.count();
}

QVariant TeamCompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mTeams.count()) {
        return {};
    }
    const TeamCompleter &room = mTeams.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case TeamName:
        return room.name();
    case TeamId:
        return room.teamId();
    case Qt::DecorationRole:
    case TeamIcon:
        return {};
    }

    return {};
}
