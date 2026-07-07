/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "markasfavoriteroomcheck.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "room.h"
#include "rooms/roomfavoritejob.h"
#include <KLocalizedString>

QString MarkAsFavoriteRoomCheck::name() const
{
    return i18n("Change Favorite Room State");
}

void MarkAsFavoriteRoomCheck::changeFavorite(bool state)
{
    auto job = new RocketChatRestApi::RoomFavoriteJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    job->setRoomId(QString::fromLatin1(mRoomId));
    job->setFavorite(state);
    if (!job->start()) {
        job->deleteLater();
    }
}

void MarkAsFavoriteRoomCheck::doStart()
{
    Room *room = mAccount ? mAccount->room(mRoomId) : nullptr;
    if (!room) {
        reportFailed(i18n("Could not find the room."));
        return;
    }
    mOriginalFavoriteState = room->favorite();
    connect(room, &Room::favoriteChanged, this, [this]() {
        checkState();
    });
    changeFavorite(!mOriginalFavoriteState);
}

void MarkAsFavoriteRoomCheck::checkState()
{
    if (isFinished()) {
        return;
    }
    Room *room = mAccount ? mAccount->room(mRoomId) : nullptr;
    if (!room || room->favorite() == mOriginalFavoriteState) {
        return; // change not reflected yet
    }
    reportPassed();
    changeFavorite(mOriginalFavoriteState); // restore
}

#include "moc_markasfavoriteroomcheck.cpp"
