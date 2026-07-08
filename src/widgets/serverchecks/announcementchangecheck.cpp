/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "announcementchangecheck.h"

#include "channels/changechannelannouncementjob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "room.h"

#include <KLocalizedString>
#include <QDateTime>

using namespace Qt::Literals::StringLiterals;

QString AnnouncementChangeCheck::name() const
{
    return i18n("Change channel announcement");
}

void AnnouncementChangeCheck::sendAnnouncement(const QString &topic)
{
    auto job = new RocketChatRestApi::ChangeChannelAnnouncementJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    job->setRoomId(QString::fromLatin1(mRoomId));
    job->setAnnouncement(topic);
    if (!job->start()) {
        job->deleteLater();
    }
}

void AnnouncementChangeCheck::doStart()
{
    Room *room = mAccount ? mAccount->room(mRoomId) : nullptr;
    if (!room) {
        reportFailed(i18n("Could not find the room."));
        return;
    }
    mOriginalAnnouncement = room->announcement();
    mNewAnnouncement = u"ruqola check announcement %1"_s.arg(QDateTime::currentMSecsSinceEpoch());
    connect(room, &Room::announcementChanged, this, [this]() {
        checkState();
    });
    sendAnnouncement(mNewAnnouncement);
}

void AnnouncementChangeCheck::checkState()
{
    if (isFinished()) {
        return;
    }
    Room *room = mAccount ? mAccount->room(mRoomId) : nullptr;
    if (!room || room->announcement() != mNewAnnouncement) {
        return; // change not reflected yet
    }
    reportPassed();
    sendAnnouncement(mOriginalAnnouncement); // restore
}

#include "moc_announcementchangecheck.cpp"
