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
