/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "videoconference/videoconference.h"
#include <QJsonArray>
#include <QObject>
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT VideoConferenceManager : public QObject
{
    Q_OBJECT
public:
    explicit VideoConferenceManager(RocketChatAccount *account, QObject *parent = nullptr);
    ~VideoConferenceManager() override;

    void parseVideoConference(const QJsonArray &contents);

Q_SIGNALS:
    void videoConferenceCallAccepted(const VideoConference &videoConference);
    void videoConferenceCallRejected(const VideoConference &videoConference);
    void videoConferenceCallConfirmed(const VideoConference &videoConference);
    void videoConferenceCallCanceled(const VideoConference &videoConference);

private:
    LIBRUQOLACORE_NO_EXPORT void showNotification(const VideoConference &videoConference);
    QList<VideoConference> mVideoConferenceList;
    RocketChatAccount *const mRocketChatAccount;
};
