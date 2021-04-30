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
    case TeamRoomsRoles::Name: {
        const QString name = teamroom.name() + (teamroom.autoJoin() ? QLatin1Char(' ') + i18n("(Autojoin)") : QString());
        return name;
    }
    case TeamRoomsRoles::AutoJoin:
        return teamroom.autoJoin();
    case TeamRoomsRoles::Identifier:
        return teamroom.identifier();
    case Qt::CheckStateRole: {
        if (mIsCheckable) {
            const QString roomId = data(index, TeamRoomsModel::Identifier).toString();
            return mRoomSelected.contains(roomId) ? Qt::Checked : Qt::Unchecked;
        }
    }
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

void TeamRoomsModel::setRoomChanged(const TeamRoom &t)
{
    const int roomCount = mTeamRooms.count();
    for (int i = 0; i < roomCount; ++i) {
        TeamRoom &teamRoom = mTeamRooms[i];
        if (teamRoom.identifier() == t.identifier()) {
            teamRoom.setAutoJoin(t.autoJoin());
            const QModelIndex idx = createIndex(i, 0);
            Q_EMIT dataChanged(idx, idx);
            break;
        }
    }
}

void TeamRoomsModel::insertRooms(const QVector<TeamRoom> &teamRooms)
{
    const int count = mTeamRooms.count();
    beginInsertRows(QModelIndex(), count, count + teamRooms.count() - 1);
    mTeamRooms.append(teamRooms);
    endInsertRows();
}

bool TeamRoomsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (mIsCheckable) {
        if (role == Qt::CheckStateRole) {
            if (index.isValid()) {
                Q_EMIT dataChanged(index, index);
                const QString roomId = data(index, TeamRoomsModel::Identifier).toString();
                if (value == Qt::Checked) {
                    mRoomSelected.append(roomId);
                } else {
                    mRoomSelected.removeAll(roomId);
                }
                return true;
            }
        }
    }
    return QAbstractListModel::setData(index, value, role);
}

Qt::ItemFlags TeamRoomsModel::flags(const QModelIndex &index) const
{
    if (mIsCheckable) {
        if (index.isValid()) {
            return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable;
        } else {
            return QAbstractListModel::flags(index);
        }
    }
    return QAbstractListModel::flags(index);
}

bool TeamRoomsModel::isCheckable() const
{
    return mIsCheckable;
}

void TeamRoomsModel::setIsCheckable(bool isCheckable)
{
    mIsCheckable = isCheckable;
}
