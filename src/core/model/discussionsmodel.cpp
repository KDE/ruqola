/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussionsmodel.h"
#include "discussions/discussions.h"

DiscussionsModel::DiscussionsModel(QObject *parent)
    : QAbstractListModel(parent)
    , mDiscussions(new Discussions)
{
}

DiscussionsModel::~DiscussionsModel()
{
    delete mDiscussions;
}

void DiscussionsModel::checkFullList()
{
    setHasFullList(mDiscussions->discussions().count() == mDiscussions->total());
}

bool DiscussionsModel::loadMoreDiscussionsInProgress() const
{
    return mLoadMoreDiscussionsInProgress;
}

void DiscussionsModel::setLoadMoreDiscussionsInProgress(bool loadMoreDiscussionsInProgress)
{
    if (mLoadMoreDiscussionsInProgress != loadMoreDiscussionsInProgress) {
        mLoadMoreDiscussionsInProgress = loadMoreDiscussionsInProgress;
        Q_EMIT loadingInProgressChanged();
    }
}

void DiscussionsModel::initialize()
{
    mRoomId.clear();
    mLoadMoreDiscussionsInProgress = false;
    setHasFullList(false);
}

QString DiscussionsModel::roomId() const
{
    return mRoomId;
}

void DiscussionsModel::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

int DiscussionsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mDiscussions->count();
}

void DiscussionsModel::addMoreDiscussions(const QJsonObject &discussionsObj)
{
    const int numberOfElement = mDiscussions->discussions().count();
    mDiscussions->parseMoreDiscussions(discussionsObj);
    beginInsertRows(QModelIndex(), numberOfElement, mDiscussions->discussions().count() - 1);
    endInsertRows();
    checkFullList();
}

Discussions *DiscussionsModel::discussions() const
{
    return mDiscussions;
}

int DiscussionsModel::total() const
{
    if (mDiscussions) {
        return mDiscussions->total();
    }
    return -1;
}

void DiscussionsModel::clear()
{
    beginResetModel();
    mRoomId.clear();
    mDiscussions->clear();
    endResetModel();
}

void DiscussionsModel::parseDiscussions(const QJsonObject &discussionsObj, const QString &roomId)
{
    mRoomId = roomId;
    if (!mDiscussions->isEmpty()) {
        beginResetModel();
        mDiscussions->clear();
        endResetModel();
    }
    mDiscussions->parseDiscussions(discussionsObj);
    if (!mDiscussions->isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mDiscussions->discussions().count() - 1);
        endInsertRows();
    }
    checkFullList();
}

QVariant DiscussionsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mDiscussions->count()) {
        return {};
    }
    const Discussion discussion = mDiscussions->at(index.row());
    switch (role) {
    case ParentId:
        return discussion.parentRoomId();
    case Description:
        return discussion.description().isEmpty() ? discussion.fname() : discussion.description();
    case NumberOfMessages:
        return discussion.numberMessages();
    case Qt::DisplayRole:
    case LastMessage:
        return discussion.lastMessageDisplay();
    case DiscussionRoomId:
        return discussion.discussionRoomId();
    case TimeStamp:
        return discussion.timeStampDisplay();
    case SortByTimeStamp:
        return discussion.timeStamp();
    case UserName:
        return discussion.userName();
    }
    return {};
}

void DiscussionsModel::setDiscussions(const Discussions &discussions)
{
    if (rowCount() != 0) {
        beginResetModel();
        mDiscussions->clear();
        endResetModel();
    }
    if (!discussions.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, discussions.count() - 1);
        mDiscussions->setDiscussions(discussions.discussions());
        endInsertRows();
    }
    checkFullList();
}

void DiscussionsModel::setHasFullList(bool state)
{
    if (mHasFullList != state) {
        mHasFullList = state;
        Q_EMIT hasFullListChanged();
    }
}

bool DiscussionsModel::hasFullList() const
{
    return mHasFullList;
}

#include "moc_discussionsmodel.cpp"
