/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencenotificationjob.h"
#include "rocketchataccount.h"
#include "ruqola_videoconference_core_debug.h"
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
        qCWarning(RUQOLA_VIDEO_CONFERENCE_LOG) << "Impossible to start VideoConferenceNotificationJob";
        deleteLater();
        return;
    }
    switch (mVideoConference.action()) {
    case VideoConference::IncomingCall:
        inComingCall();
        return;
    case VideoConference::Unknown:
    case VideoConference::Canceled:
    case VideoConference::Confirmed:
    case VideoConference::Accepted:
    case VideoConference::Rejected:
        break;
    }
    deleteLater();
}

QString VideoConferenceNotificationJob::generateText() const
{
    const QString str = mRocketChatAccount->accountName() + QLatin1Char('\n');
    // Add user name!
    return str;
}

void VideoConferenceNotificationJob::inComingCall()
{
    auto notification = new KNotification(QStringLiteral("VideoConference-Incoming"), KNotification::CloseOnTimeout);
    notification->setTitle(i18n("InComing Call"));
    // notification->setIconName(QStringLiteral("network-connect"));
    notification->setText(generateText());

#if QT_VERSION_MAJOR == 6
    auto acceptAction = notification->addAction(i18n("Accept"));
    connect(acceptAction, &KNotificationAction::activated, this, [this] {
        Q_EMIT acceptVideoConference();
        deleteLater();
    });

    auto rejectAction = notification->addAction(i18n("Reject"));
    connect(rejectAction, &KNotificationAction::activated, this, [this] {
        Q_EMIT rejectVideoConference();
        deleteLater();
    });
#else
    const QStringList lstActions{i18n("Accept"), i18n("Reject")};
    notification->setActions(lstActions);
    connect(notification, &KNotification::activated, this, [this](uint val) {
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
    });
#endif

    connect(notification, &KNotification::closed, this, &VideoConferenceNotificationJob::deleteLater);
    notification->sendEvent();
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

#include "moc_videoconferencenotificationjob.cpp"
