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

DirectoryUsersModel::DirectoryUsersModel(QObject *parent)
    : DirectoryBaseModel(parent)
{
}

DirectoryUsersModel::~DirectoryUsersModel()
{
}

void DirectoryUsersModel::checkFullList()
{
    // setHasFullList(mDiscussions->discussions().count() == mDiscussions->total());
}

int DirectoryUsersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    // return mDiscussions->count();
    return {};
}

void DirectoryUsersModel::parseElements(const QJsonObject &obj)
{
    Q_EMIT totalChanged();
}

void DirectoryUsersModel::addMoreElements(const QJsonObject &obj)
{
}

QVariant DirectoryUsersModel::data(const QModelIndex &index, int role) const
{
    //    if (index.row() < 0 || index.row() >= mDiscussions->count()) {
    //        return {};
    //    }
    //    const Discussion discussion = mDiscussions->at(index.row());
    //    switch (role) {
    //    case ParentId:
    //        return discussion.parentRoomId();
    //    case Description:
    //        return discussion.description().isEmpty() ? discussion.fname() : discussion.description();
    //    case NumberOfMessages:
    //        return discussion.numberMessages();
    //    case Qt::DisplayRole:
    //    case LastMessage:
    //        return discussion.lastMessageDisplay();
    //    case DiscussionRoomId:
    //        return discussion.discussionRoomId();
    //    case TimeStamp:
    //        return discussion.timeStampDisplay();
    //    case SortByTimeStamp:
    //        return discussion.timeStamp();
    //    }
    return {};
}

QVariant DirectoryUsersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        //        switch (static_cast<AdminRoomsRoles>(section)) {
        //        case AdminRoomsRoles::Name:
        //            return i18n("Name");
        //        case AdminRoomsRoles::MessagesCount:
        //            return i18n("Number Of Messages");
        //        case AdminRoomsRoles::UsersCount:
        //            return i18n("Number Of Users");
        //        case AdminRoomsRoles::Topic:
        //            return i18n("Topic");
        //        case AdminRoomsRoles::Identifier:
        //            return i18n("Identifier");
        //        case AdminRoomsRoles::ReadOnly:
        //            return i18n("Read Only");
        //        case AdminRoomsRoles::DefaultRoom:
        //            return i18n("Default Room");
        //        case AdminRoomsRoles::ChannelType:
        //            return i18n("Type");
        //        case AdminRoomsRoles::ChannelTypeStr:
        //            return i18n("Type");
        //        }
    }
    return QVariant();
}

int DirectoryUsersModel::total() const
{
    return -1;
}
