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

DirectoryRoomsModel::DirectoryRoomsModel(QObject *parent)
    : DirectoryBaseModel(parent)
{
}

DirectoryRoomsModel::~DirectoryRoomsModel()
{
}

void DirectoryRoomsModel::checkFullList()
{
    // setHasFullList(mDiscussions->discussions().count() == mDiscussions->total());
}

int DirectoryRoomsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    // return mDiscussions->count();
    return {};
}

void DirectoryRoomsModel::addMoreDiscussions(const QJsonObject &discussionsObj)
{
    //    const int numberOfElement = mDiscussions->discussions().count();
    //    mDiscussions->parseMoreDiscussions(discussionsObj);
    //    beginInsertRows(QModelIndex(), numberOfElement, mDiscussions->discussions().count() - 1);
    //    endInsertRows();
    //    checkFullList();
}

void DirectoryRoomsModel::parseDiscussions(const QJsonObject &discussionsObj, const QString &roomId)
{
    //    mRoomId = roomId;
    //    if (rowCount() != 0) {
    //        beginRemoveRows(QModelIndex(), 0, mDiscussions->discussions().count() - 1);
    //        mDiscussions->clear();
    //        endRemoveRows();
    //    }
    //    mDiscussions->parseDiscussions(discussionsObj);
    //    if (!mDiscussions->isEmpty()) {
    //        beginInsertRows(QModelIndex(), 0, mDiscussions->discussions().count() - 1);
    //        endInsertRows();
    //    }
    //    checkFullList();
}

QVariant DirectoryRoomsModel::data(const QModelIndex &index, int role) const
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
