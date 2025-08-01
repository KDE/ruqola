/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencesmodel.h"
#include <KLocalizedString>

VideoConferencesModel::VideoConferencesModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mRochetChantAccount(account)
{
}

VideoConferencesModel::~VideoConferencesModel() = default;

void VideoConferencesModel::checkFullList()
{
    setHasFullList(mVideoConferenceInfos.count() == mVideoConferenceInfos.total());
}

bool VideoConferencesModel::loadMoreVideoConferencesInProgress() const
{
    return mLoadMoreFilesInProgress;
}

void VideoConferencesModel::setLoadMoreVideoConferencesInProgress(bool loadMoreFilesInProgress)
{
    if (mLoadMoreFilesInProgress != loadMoreFilesInProgress) {
        mLoadMoreFilesInProgress = loadMoreFilesInProgress;
        Q_EMIT loadingInProgressChanged();
    }
}

void VideoConferencesModel::clear()
{
    beginResetModel();
    mVideoConferenceInfos.clear();
    endResetModel();
}

void VideoConferencesModel::addMoreVideoConferences(const QJsonObject &fileAttachmentsObj)
{
    const int numberOfElement = mVideoConferenceInfos.count();
    mVideoConferenceInfos.parseMoreVideoConferenceInfos(fileAttachmentsObj);
    beginInsertRows(QModelIndex(), numberOfElement, mVideoConferenceInfos.count() - 1);
    endInsertRows();
    checkFullList();
}

void VideoConferencesModel::initialize()
{
    mRoomId.clear();
    mLoadMoreFilesInProgress = false;
    setHasFullList(false);
}

void VideoConferencesModel::parseVideoConferences(const QJsonObject &fileAttachmentsObj, const QString &roomId)
{
    mRoomId = roomId;
    if (rowCount() != 0) {
        clear();
    }
    mVideoConferenceInfos.parseVideoConferenceInfos(fileAttachmentsObj);
    if (!mVideoConferenceInfos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mVideoConferenceInfos.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

QString VideoConferencesModel::roomId() const
{
    return mRoomId;
}

void VideoConferencesModel::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

void VideoConferencesModel::setVideoConferenceInfos(const QList<VideoConferenceInfo> &files)
{
    clear();
    if (!files.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, files.count() - 1);
        mVideoConferenceInfos.setVideoConferenceInfosList(files);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

int VideoConferencesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }
    return mVideoConferenceInfos.count();
}

QVariant VideoConferencesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mVideoConferenceInfos.count()) {
        return {};
    }
    /*
        const VideoConferenceInfo &file = mVideoConferenceInfos.at(index.row());
        switch (role) {
        // TODO
        default:
            break;
        }
        */
    return {};
}

int VideoConferencesModel::total() const
{
    return mVideoConferenceInfos.count();
}

void VideoConferencesModel::setHasFullList(bool state)
{
    if (mHasFullList != state) {
        mHasFullList = state;
        Q_EMIT hasFullListChanged();
    }
}

bool VideoConferencesModel::hasFullList() const
{
    return mHasFullList;
}

#include "moc_videoconferencesmodel.cpp"
