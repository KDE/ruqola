/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencemanager.h"
#include <QJsonObject>

VideoConferenceManager::VideoConferenceManager(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mRocketChatAccount(account)
{
}

VideoConferenceManager::~VideoConferenceManager() = default;

void VideoConferenceManager::parseVideoConference(const QJsonArray &contents)
{
    for (const QJsonValue &current : contents) {
        const QJsonObject videoConfObject = current.toObject();
        VideoConference videoConference;
        videoConference.parseVideoConference(videoConfObject);
        if (videoConference.isValid()) {
            mVideoConferenceList.append(std::move(videoConference));
        }
    }
    // [{"action":"call","params":{"callId":"63983180a7f9e1466a4eedc6","rid":"YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K","uid":"YbwG4T2uB3wZSZSKB"}}]
    // TODO
}
