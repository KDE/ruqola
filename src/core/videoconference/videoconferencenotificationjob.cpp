/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencenotificationjob.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include <KLocalizedString>
#include <KNotification>

VideoConferenceNotificationJob::VideoConferenceNotificationJob(QObject *parent)
    : QObject{parent}
{
}

VideoConferenceNotificationJob::~VideoConferenceNotificationJob() = default;

void VideoConferenceNotificationJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_LOG) << "Impossible to start VideoConferenceNotificationJob";
        deleteLater();
        return;
    }
    switch (mVideoConference.action()) {
    case VideoConference::IncomingCall:
        inComingCall();
        break;
    case VideoConference::Unknown:
        break;
    case VideoConference::Canceled:
        videoConferenceCanceled();
        break;
    case VideoConference::Confirmed:
        videoConferenceConfirmed();
        break;
    case VideoConference::Accepted:
        videoConferenceAccepted();
        break;
    case VideoConference::Rejected:
        videoConferenceRejected();
        break;
    }
}

void VideoConferenceNotificationJob::videoConferenceRejected()
{
    // TODO
}

void VideoConferenceNotificationJob::videoConferenceAccepted()
{
    // TODO
}

void VideoConferenceNotificationJob::videoConferenceConfirmed()
{
    // TODO
}

void VideoConferenceNotificationJob::videoConferenceCanceled()
{
    // TODO
}

void VideoConferenceNotificationJob::inComingCall()
{
    auto notification = new KNotification(QStringLiteral("VideoConference-Incoming"), KNotification::CloseOnTimeout);
    notification->setTitle(i18n("InComing Call"));
    // notification->setIconName(QStringLiteral("network-connect"));
    // notification->setText(generateText());
    const QStringList lstActions{i18n("Accept"), i18n("Reject")};
    notification->setActions(lstActions);

    connect(notification, qOverload<unsigned int>(&KNotification::activated), this, &VideoConferenceNotificationJob::slotActivateNotificationAction);
    connect(notification, &KNotification::closed, this, &VideoConferenceNotificationJob::deleteLater);
    notification->sendEvent();
}

void VideoConferenceNotificationJob::slotActivateNotificationAction(unsigned int val)
{
    // Index == 0 => is the default action. We don't have it.
    switch (val) {
    case 0:
        break;
    case 1:
        Q_EMIT acceptVideoConference();
        break;
    case 2:
        Q_EMIT rejectVideoConference();
        break;
    }
    deleteLater();
}

bool VideoConferenceNotificationJob::canStart() const
{
    return mVideoConference.isValid();
}

void VideoConferenceNotificationJob::setRocketChatAccount(RocketChatAccount *account)
{
    mRocketChatAccount = account;
}

VideoConference VideoConferenceNotificationJob::videoConference() const
{
    return mVideoConference;
}

void VideoConferenceNotificationJob::setVideoConference(const VideoConference &newVideoConference)
{
    mVideoConference = newVideoConference;
}
