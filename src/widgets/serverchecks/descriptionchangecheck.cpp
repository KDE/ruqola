/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "descriptionchangecheck.h"

#include "channels/changechanneldescriptionjob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "room.h"

#include <KLocalizedString>
#include <QDateTime>

using namespace Qt::Literals::StringLiterals;

QString DescriptionChangeCheck::name() const
{
    return i18n("Change channel description");
}

void DescriptionChangeCheck::sendDescription(const QString &topic)
{
    auto job = new RocketChatRestApi::ChangeChannelDescriptionJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    job->setRoomId(QString::fromLatin1(mRoomId));
    job->setDescription(topic);
    if (!job->start()) {
        job->deleteLater();
    }
}

void DescriptionChangeCheck::doStart()
{
    Room *room = mAccount ? mAccount->room(mRoomId) : nullptr;
    if (!room) {
        reportFailed(i18n("Could not find the room."));
        return;
    }
    mOriginalDescription = room->description();
    mNewDescription = u"ruqola check description %1"_s.arg(QDateTime::currentMSecsSinceEpoch());
    connect(room, &Room::descriptionChanged, this, [this]() {
        checkState();
    });
    sendDescription(mNewDescription);
}

void DescriptionChangeCheck::checkState()
{
    if (isFinished()) {
        return;
    }
    Room *room = mAccount ? mAccount->room(mRoomId) : nullptr;
    if (!room || room->description() != mNewDescription) {
        return; // change not reflected yet
    }
    reportPassed();
    sendDescription(mOriginalDescription); // restore
}

#include "moc_descriptionchangecheck.cpp"
