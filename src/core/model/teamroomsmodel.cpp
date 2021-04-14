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

#include "teamroomsmodel.h"
#include <KLocalizedString>

TeamRoomsModel::TeamRoomsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

TeamRoomsModel::~TeamRoomsModel()
{
}

int TeamRoomsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mTeamRooms.count();
}

QVariant TeamRoomsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mTeamRooms.count()) {
        return {};
    }
    const TeamRoom &teamroom = mTeamRooms.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case TeamRoomsRoles::Name:
        return teamroom.name();
    case TeamRoomsRoles::AutoJoin:
        return teamroom.autoJoin();
    case TeamRoomsRoles::Identifier:
        return teamroom.identifier();
    }
    return {};
}

QVector<TeamRoom> TeamRoomsModel::teamRooms() const
{
    return mTeamRooms;
}

void TeamRoomsModel::setTeamRooms(const QVector<TeamRoom> &teamRooms)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mTeamRooms.count() - 1);
        mTeamRooms.clear();
        endRemoveRows();
    }
    if (!teamRooms.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, teamRooms.count() - 1);
        mTeamRooms = teamRooms;
        endInsertRows();
    }
}
