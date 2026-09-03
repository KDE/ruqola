/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

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
    VideoConferenceInfos videoConferenceInfos = mVideoConferenceInfos;
    videoConferenceInfos.parseMoreVideoConferenceInfos(fileAttachmentsObj);
    const int newNumberOfElement = videoConferenceInfos.count();
    if (newNumberOfElement > numberOfElement) {
        beginInsertRows(QModelIndex(), numberOfElement, newNumberOfElement - 1);
        mVideoConferenceInfos = std::move(videoConferenceInfos);
        endInsertRows();
    } else { // No new element but offset/total may have changed
        mVideoConferenceInfos = std::move(videoConferenceInfos);
    }
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
    beginResetModel();
    mVideoConferenceInfos.clear();
    mVideoConferenceInfos.parseVideoConferenceInfos(fileAttachmentsObj);
    endResetModel();
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

void VideoConferencesModel::setVideoConferenceInfos(QList<VideoConferenceInfo> files)
{
    beginResetModel();
    mVideoConferenceInfos.setVideoConferenceInfosList(std::move(files));
    endResetModel();
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
