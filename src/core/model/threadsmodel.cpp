/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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
#include "threads.h"
#include <QDateTime>

ThreadsModel::ThreadsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    mThreads = new Threads;
}

ThreadsModel::~ThreadsModel()
{
    delete mThreads;
}

void ThreadsModel::checkFullList()
{
    setHasFullList(mThreads->count() == mThreads->total());
}

bool ThreadsModel::loadMoreThreadsInProgress() const
{
    return mLoadMoreThreadsInProgress;
}

void ThreadsModel::setLoadMoreThreadsInProgress(bool loadMoreThreadsInProgress)
{
    mLoadMoreThreadsInProgress = loadMoreThreadsInProgress;
}

void ThreadsModel::initialize()
{
    mRoomId.clear();
    setHasFullList(false);
    mLoadMoreThreadsInProgress = false;
}

int ThreadsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mThreads->count();
}

QVariant ThreadsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mThreads->count()) {
        return {};
    }
    const Thread thread = mThreads->at(index.row());
    switch (role) {
    case ThreadMessageId:
        return thread.messageId();
    case Description:
        return thread.text();
    case NumberOfMessages:
        return thread.threadCount();
    case LastMessage:
        //Need to cache it
        return QDateTime::fromMSecsSinceEpoch(thread.threadLastMessage()).toString(Qt::SystemLocaleLongDate);
    case TimeStamp:
        return thread.timeStamp();
    case UserName:
        return thread.username();
    case AliasName:
        return thread.alias();
    case Avatar:
        return thread.avatar();
    case UserId:
        return thread.userId();
    case ThreadMessagePreview:
        return threadMessagePreview(thread.text());
    case ShowTranslatedMessage:
        return thread.showTranslatedMessage();
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
    roles[TimeStamp] = QByteArrayLiteral("timestamp");
    roles[UserName] = QByteArrayLiteral("username");
    roles[AliasName] = QByteArrayLiteral("aliasname");
    roles[Avatar] = QByteArrayLiteral("avatar");
    roles[UserId] = QByteArrayLiteral("userid");
    roles[ThreadMessagePreview] = QByteArrayLiteral("threadMessagePreview");
    roles[ShowTranslatedMessage] = QByteArrayLiteral("showTranslatedMessage");
    return roles;
}

void ThreadsModel::setThreads(const Threads &threads)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mThreads->count() - 1);
        mThreads->clear();
        endRemoveRows();
    }
    if (!threads.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, threads.count() - 1);
        mThreads->setThreads(threads.threads());
        endInsertRows();
    }
    checkFullList();
}

QString ThreadsModel::roomId() const
{
    return mRoomId;
}

void ThreadsModel::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

void ThreadsModel::addMoreThreads(const QJsonObject &threadsObj)
{
    const int numberOfElement = mThreads->count();
    mThreads->parseMoreThreads(threadsObj);
    beginInsertRows(QModelIndex(), numberOfElement, mThreads->count() - 1);
    endInsertRows();
    checkFullList();
}

Threads *ThreadsModel::threads() const
{
    return mThreads;
}

void ThreadsModel::parseThreads(const QJsonObject &threadsObj, const QString &roomId)
{
    mRoomId = roomId;
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mThreads->count() - 1);
        mThreads->clear();
        endRemoveRows();
    }
    mThreads->parseThreads(threadsObj);
    if (!mThreads->isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mThreads->count() - 1);
        endInsertRows();
    }
    checkFullList();
}

int ThreadsModel::total() const
{
    if (mThreads) {
        return mThreads->total();
    }
    return -1;
}

void ThreadsModel::setHasFullList(bool hasFullList)
{
    if (mHasFullList != hasFullList) {
        mHasFullList = hasFullList;
        Q_EMIT hasFullListChanged();
    }
}

bool ThreadsModel::hasFullList() const
{
    return mHasFullList;
}

QString ThreadsModel::threadMessagePreview(const QString &messageStr) const
{
    QString str = messageStr;
    if (str.length() > 80) {
        str = str.left(80) + QStringLiteral("...");
    }
    return str;
}
