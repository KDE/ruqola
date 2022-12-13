/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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
    Q_REQUIRED_RESULT bool canStart() const;
    void setRocketChatAccount(RocketChatAccount *account);

    Q_REQUIRED_RESULT VideoConference videoConference() const;
    void setVideoConference(const VideoConference &newVideoConference);

Q_SIGNALS:
    void acceptVideoConference();
    void rejectVideoConference();

private:
    void slotActivateNotificationAction(unsigned int val);
    void inComingCall();
    void videoConferenceAccepted();
    void videoConferenceConfirmed();
    void videoConferenceCanceled();
    VideoConference mVideoConference;
    QPointer<RocketChatAccount> mRocketChatAccount;
};
