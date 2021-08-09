/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "directoryteamsmodel.h"
#include "discussions.h"
#include <KLocalizedString>

DirectoryTeamsModel::DirectoryTeamsModel(QObject *parent)
    : DirectoryBaseModel(parent)
{
}

DirectoryTeamsModel::~DirectoryTeamsModel()
{
}

void DirectoryTeamsModel::checkFullList()
{
    setHasFullList(mRoomsInfo.count() == mRoomsInfo.total());
}

int DirectoryTeamsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mRoomsInfo.count();
}

QList<int> DirectoryTeamsModel::hideColumns() const
{
    return {TeamIdentifier};
}

void DirectoryTeamsModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mRoomsInfo.count();
    mRoomsInfo.parseMoreRooms(obj, RoomsInfo::Directory);
    beginInsertRows(QModelIndex(), numberOfElement, mRoomsInfo.count() - 1);
    endInsertRows();
    checkFullList();
}

void DirectoryTeamsModel::parseElements(const QJsonObject &obj)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mRoomsInfo.count() - 1);
        mRoomsInfo.clear();
        endRemoveRows();
    }
    mRoomsInfo.parseRooms(obj, RoomsInfo::Directory);
    if (!mRoomsInfo.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mRoomsInfo.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

QVariant DirectoryTeamsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRoomsInfo.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }
    const RoomInfo &roomInfo = mRoomsInfo.at(index.row());
    const int col = index.column();
    switch (static_cast<DirectoryTeamsRoles>(col)) {
    case DirectoryTeamsRoles::TeamName:
        return roomInfo.roomName();
    case DirectoryTeamsRoles::RoomsCount:
        return roomInfo.teamInfo().roomsCount();
    case DirectoryTeamsRoles::TeamIdentifier:
        return roomInfo.teamInfo().teamId();
    case DirectoryTeamsRoles::CreateTeam:
        return roomInfo.createdRoomDisplayDateTimeStr();
    }
    return {};
}

QVariant DirectoryTeamsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<DirectoryTeamsRoles>(section)) {
        case DirectoryTeamsModel::TeamName:
            return i18n("Name");
        case DirectoryTeamsRoles::RoomsCount:
            return i18n("Rooms");
        case DirectoryTeamsRoles::TeamIdentifier:
            return i18n("Identifier");
        case DirectoryTeamsRoles::CreateTeam:
            return i18n("Created");
        }
    }
    return QVariant();
}

int DirectoryTeamsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(DirectoryTeamsModel::LastColumn) + 1;
}

int DirectoryTeamsModel::total() const
{
    return mRoomsInfo.total();
}
