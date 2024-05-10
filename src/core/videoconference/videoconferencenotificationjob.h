/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "videoconference.h"
#include <QObject>
#include <QPointer>
class RocketChatAccount;
class LIBRUQOLACORE_TESTS_EXPORT VideoConferenceNotificationJob : public QObject
{
    Q_OBJECT
public:
    explicit VideoConferenceNotificationJob(QObject *parent = nullptr);
    ~VideoConferenceNotificationJob() override;

    void start();
    [[nodiscard]] bool canStart() const;
    void setRocketChatAccount(RocketChatAccount *account);

    [[nodiscard]] VideoConference videoConference() const;
    void setVideoConference(const VideoConference &newVideoConference);

Q_SIGNALS:
    void acceptVideoConference();
    void rejectVideoConference();

private:
    LIBRUQOLACORE_NO_EXPORT void inComingCall();
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString generateText() const;
    VideoConference mVideoConference;
    QPointer<RocketChatAccount> mRocketChatAccount;
};
