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

#include "directoryroomsmodel.h"
#include "discussions.h"
#include <KLocalizedString>

DirectoryRoomsModel::DirectoryRoomsModel(QObject *parent)
    : DirectoryBaseModel(parent)
{
}

DirectoryRoomsModel::~DirectoryRoomsModel()
{
}

void DirectoryRoomsModel::checkFullList()
{
    setHasFullList(mRoomsInfo.count() == mRoomsInfo.total());
}

int DirectoryRoomsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mRoomsInfo.count();
}

void DirectoryRoomsModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mRoomsInfo.count();
    mRoomsInfo.parseMoreRooms(obj, RoomsInfo::Directory);
    beginInsertRows(QModelIndex(), numberOfElement, mRoomsInfo.count() - 1);
    endInsertRows();
    checkFullList();
}

QList<int> DirectoryRoomsModel::hideColumns() const
{
    return {Identifier, ChannelType};
}

void DirectoryRoomsModel::parseElements(const QJsonObject &roomsObj)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mRoomsInfo.count() - 1);
        mRoomsInfo.clear();
        endRemoveRows();
    }
    mRoomsInfo.parseRooms(roomsObj, RoomsInfo::Directory);
    if (!mRoomsInfo.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mRoomsInfo.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

QVariant DirectoryRoomsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRoomsInfo.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const RoomInfo &roomInfo = mRoomsInfo.at(index.row());
    const int col = index.column();
    switch (static_cast<DirectoryRoomsRoles>(col)) {
    case DirectoryRoomsRoles::Name:
        return roomInfo.roomName();
    case DirectoryRoomsRoles::UsersCount:
        return roomInfo.usersCount();
    case DirectoryRoomsRoles::Topic:
        return roomInfo.topic();
    case DirectoryRoomsRoles::Identifier:
        return roomInfo.identifier();
    case DirectoryRoomsRoles::ReadOnly:
        return roomInfo.readOnly();
    case DirectoryRoomsRoles::DefaultRoom:
        return roomInfo.defaultRoom();
    case DirectoryRoomsRoles::ChannelType:
        return roomInfo.channelType();
    case DirectoryRoomsRoles::ChannelTypeStr:
        return roomInfo.channelTypeStr();
    case DirectoryRoomsRoles::LastMessage:
        return roomInfo.lastMessageDisplayDateTimeStr();
    }
    return {};
}

QVariant DirectoryRoomsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<DirectoryRoomsRoles>(section)) {
        case DirectoryRoomsModel::Name:
            return i18n("Name");
        case DirectoryRoomsModel::UsersCount:
            return i18n("Number Of Users");
        case DirectoryRoomsModel::Topic:
            return i18n("Topic");
        case DirectoryRoomsModel::Identifier:
            return i18n("Identifier");
        case DirectoryRoomsModel::ReadOnly:
            return i18n("Read Only");
        case DirectoryRoomsModel::DefaultRoom:
            return i18n("Default Room");
        case DirectoryRoomsModel::LastMessage:
            return i18n("Last Message");
        case DirectoryRoomsModel::ChannelTypeStr:
        case DirectoryRoomsModel::ChannelType:
            return i18n("Channel Type");
        }
    }
    return QVariant();
}

int DirectoryRoomsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(DirectoryRoomsModel::LastColumn) + 1;
}

int DirectoryRoomsModel::total() const
{
    return mRoomsInfo.total();
}
