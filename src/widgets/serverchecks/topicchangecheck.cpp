/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "topicchangecheck.h"

#include "channels/changechanneltopicjob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "room.h"

#include <KLocalizedString>
#include <QDateTime>

using namespace Qt::Literals::StringLiterals;

QString TopicChangeCheck::name() const
{
    return i18n("Change channel topic");
}

void TopicChangeCheck::sendTopic(const QString &topic)
{
    auto job = new RocketChatRestApi::ChangeChannelTopicJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    job->setRoomId(QString::fromLatin1(mRoomId));
    job->setTopic(topic);
    if (!job->start()) {
        job->deleteLater();
    }
}

void TopicChangeCheck::doStart()
{
    Room *room = mAccount ? mAccount->room(mRoomId) : nullptr;
    if (!room) {
        reportFailed(i18n("Could not find the room."));
        return;
    }
    mOriginalTopic = room->topic();
    mNewTopic = u"ruqola check topic %1"_s.arg(QDateTime::currentMSecsSinceEpoch());
    connect(room, &Room::topicChanged, this, [this]() {
        checkState();
    });
    sendTopic(mNewTopic);
}

void TopicChangeCheck::checkState()
{
    if (isFinished()) {
        return;
    }
    Room *room = mAccount ? mAccount->room(mRoomId) : nullptr;
    if (!room || room->topic() != mNewTopic) {
        return; // change not reflected yet
    }
    reportPassed();
    sendTopic(mOriginalTopic); // restore
}

#include "moc_topicchangecheck.cpp"
