/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "threadsmodel.h"

#include <QDateTime>

ThreadsModel::ThreadsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

ThreadsModel::~ThreadsModel()
{
}

int ThreadsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mThreads.count();
}

QVariant ThreadsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mThreads.count()) {
        return {};
    }
    const Thread thread = mThreads.at(index.row());
    switch (role) {
    case ThreadMessageId:
        //return thread.parentRoomId();
        return {};
    case Description:
        return thread.text();
    case NumberOfMessages:
        return thread.threadCount();
    case LastMessage:
        //Need to cache it
        return QDateTime::fromMSecsSinceEpoch(thread.threadLastMessage()).toString(Qt::SystemLocaleLongDate);
    }

    return {};
}

QHash<int, QByteArray> ThreadsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ThreadMessageId] = QByteArrayLiteral("threadmessageid");
    roles[Description] = QByteArrayLiteral("description");
    roles[NumberOfMessages] = QByteArrayLiteral("numberofmessages");
    roles[LastMessage] = QByteArrayLiteral("lastmessage");
    return roles;
}

void ThreadsModel::setThreads(const Threads &threads)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mThreads.count() - 1);
        mThreads.clear();
        endRemoveRows();
    }
    if (!threads.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, threads.count() - 1);
        mThreads = threads;
        endInsertRows();
    }
}
