/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directoryteamsmodel.h"
#include <KLocalizedString>

DirectoryTeamsModel::DirectoryTeamsModel(QObject *parent)
    : DirectoryBaseModel(parent)
{
}

DirectoryTeamsModel::~DirectoryTeamsModel() = default;

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
    return {TeamIdentifier, CreateTeamDateTime, RoomIdentifier};
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
    case DirectoryTeamsRoles::RoomIdentifier:
        return roomInfo.identifier();
    case DirectoryTeamsRoles::CreateTeam:
        return roomInfo.createdRoomDisplayDateTimeStr();
    case DirectoryTeamsRoles::CreateTeamDateTime:
        return roomInfo.createdRoom();
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
        case DirectoryTeamsRoles::RoomIdentifier:
        case DirectoryTeamsRoles::CreateTeamDateTime:
            return {};
        }
    }
    return {};
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
