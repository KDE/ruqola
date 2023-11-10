/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamroomsmodel.h"
#include <KLocalizedString>

TeamRoomsModel::TeamRoomsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

TeamRoomsModel::~TeamRoomsModel() = default;

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
    if (!mTeamRooms.isEmpty()) {
        beginResetModel();
        mTeamRooms.clear();
        endResetModel();
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
                    mRoomSelected.append(std::move(roomId));
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

#include "moc_teamroomsmodel.cpp"
