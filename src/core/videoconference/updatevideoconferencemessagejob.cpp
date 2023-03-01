/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "updatevideoconferencemessagejob.h"
#include "rocketchataccount.h"
#include "room.h"
#include "ruqola_videoconference_core_debug.h"

UpdateVideoConferenceMessageJob::UpdateVideoConferenceMessageJob(QObject *parent)
    : QObject{parent}
{
}

UpdateVideoConferenceMessageJob::~UpdateVideoConferenceMessageJob() = default;

bool UpdateVideoConferenceMessageJob::canStart() const
{
    return mVideoConferenceInfo.isValid() && mRocketChatAccount;
}

void UpdateVideoConferenceMessageJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_VIDEO_CONFERENCE_LOG) << "Impossible to start UpdateVideoConferenceMessageJob";
        deleteLater();
        return;
    }
    const QString roomId{mVideoConferenceInfo.roomId()};
    auto room = mRocketChatAccount->room(roomId);
    if (room) {
        // qDebug() << " room" << room << " **** " << *room;
        auto messageModel = room->messageModel();
        // Search messages
        if (messageModel) {
            Message msg = messageModel->findMessageById(mVideoConferenceInfo.messageId());
            msg.setVideoConferenceInfo(std::move(mVideoConferenceInfo));
            // qDebug() << " msg " << msg;
            mRocketChatAccount->addMessageToDataBase(room->displayFName(), msg);
            messageModel->addMessages({msg});
            // TODO update database
        }
    } else {
        qCWarning(RUQOLA_VIDEO_CONFERENCE_LOG) << " impossible to return room " << roomId;
    }
    deleteLater();
}

VideoConferenceInfo UpdateVideoConferenceMessageJob::videoConferenceInfo() const
{
    return mVideoConferenceInfo;
}

void UpdateVideoConferenceMessageJob::setVideoConferenceInfo(const VideoConferenceInfo &newVideoConferenceInfo)
{
    mVideoConferenceInfo = newVideoConferenceInfo;
}

RocketChatAccount *UpdateVideoConferenceMessageJob::rocketChatAccount() const
{
    return mRocketChatAccount;
}

void UpdateVideoConferenceMessageJob::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}
