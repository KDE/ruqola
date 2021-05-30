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

#include "directoryusersmodel.h"
#include "discussions.h"
#include <KLocalizedString>

DirectoryUsersModel::DirectoryUsersModel(QObject *parent)
    : DirectoryBaseModel(parent)
{
}

DirectoryUsersModel::~DirectoryUsersModel()
{
}

void DirectoryUsersModel::checkFullList()
{
    // setHasFullList(mRoomsInfo.count() == mRoomsInfo.total());
}

int DirectoryUsersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    // return mRoomsInfo.count();
    return {};
}

QList<int> DirectoryUsersModel::hideColumns() const
{
    return {};
}

void DirectoryUsersModel::addMoreElements(const QJsonObject &obj)
{
    //    const int numberOfElement = mRoomsInfo.count();
    //    mRoomsInfo.parseMoreRooms(obj, RoomsInfo::Directory);
    //    beginInsertRows(QModelIndex(), numberOfElement, mRoomsInfo.count() - 1);
    //    endInsertRows();
    checkFullList();
}

void DirectoryUsersModel::parseElements(const QJsonObject &discussionsObj)
{
    //    if (rowCount() != 0) {
    //        beginRemoveRows(QModelIndex(), 0, mRoomsInfo.count() - 1);
    //        mRoomsInfo.clear();
    //        endRemoveRows();
    //    }
    //    mRoomsInfo.parseRooms(discussionsObj, RoomsInfo::Directory);
    //    if (!mRoomsInfo.isEmpty()) {
    //        beginInsertRows(QModelIndex(), 0, mRoomsInfo.count() - 1);
    //        endInsertRows();
    //    }
    //    checkFullList();
    //    Q_EMIT totalChanged();
}

QVariant DirectoryUsersModel::data(const QModelIndex &index, int role) const
{
    //    if (index.row() < 0 || index.row() >= mRoomsInfo.count()) {
    //        return {};
    //    }
    //    if (role != Qt::DisplayRole) {
    //        return {};
    //    }
    //    const RoomInfo &roomInfo = mRoomsInfo.at(index.row());
    //    const int col = index.column();
    //    switch (static_cast<DirectoryTeamsRoles>(col)) {
    //    case DirectoryTeamsRoles::TeamName:
    //        return roomInfo.roomName();
    //    case DirectoryTeamsRoles::RoomsCount:
    //        return roomInfo.teamInfo().roomsCount();
    //    }
    return {};
}

QVariant DirectoryUsersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<DirectoryUsersRoles>(section)) {
        case DirectoryUsersModel::Name:
            return i18n("Name");
        case DirectoryUsersModel::Email:
            return i18n("Emails");
        case DirectoryUsersModel::JoinAt:
            return i18n("Emails");
        }
    }
    return QVariant();
}

int DirectoryUsersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(DirectoryUsersModel::LastColumn) + 1;
}

int DirectoryUsersModel::total() const
{
    // return mRoomsInfo.total();
    return -1;
}
