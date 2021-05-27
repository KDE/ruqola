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

void DirectoryTeamsModel::addMoreTeams(const QJsonObject &obj)
{
    const int numberOfElement = mRoomsInfo.count();
    mRoomsInfo.parseMoreRooms(obj);
    beginInsertRows(QModelIndex(), numberOfElement, mRoomsInfo.count() - 1);
    endInsertRows();
    checkFullList();
}

void DirectoryTeamsModel::parseTeams(const QJsonObject &discussionsObj)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mRoomsInfo.count() - 1);
        mRoomsInfo.clear();
        endRemoveRows();
    }
    mRoomsInfo.parseRooms(discussionsObj);
    if (!mRoomsInfo.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mRoomsInfo.count() - 1);
        endInsertRows();
    }
    checkFullList();
}

QVariant DirectoryTeamsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRoomsInfo.count()) {
        return {};
    }
    const RoomInfo roomInfo = mRoomsInfo.at(index.row());
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
