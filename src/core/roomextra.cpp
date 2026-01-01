/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomextra.h"
#include "ruqola_memory_management_debug.h"
#include "ruqola_room_memory_debug.h"
#include <QDateTime>

RoomExtra::RoomExtra()
{
    qCDebug(RUQOLA_ROOM_MEMORY_LOG) << " RoomExtra created " << this;
}

RoomExtra::~RoomExtra()
{
    qCDebug(RUQOLA_ROOM_MEMORY_LOG) << " RoomExtra deleted " << this;
}

QString RoomExtra::autoTranslateLanguage() const
{
    return mAutotranslateLanguage;
}

void RoomExtra::setAutoTranslateLanguage(const QString &autotranslateLanguage)
{
    mAutotranslateLanguage = autotranslateLanguage;
}

QStringList RoomExtra::mutedUsers() const
{
    return mMutedUsers;
}

QStringList RoomExtra::ignoredUsers() const
{
    return mIgnoredUsers;
}

void RoomExtra::setIgnoredUsers(const QStringList &newIgnoredUsers)
{
    mIgnoredUsers = newIgnoredUsers;
}

QStringList RoomExtra::highlightsWord() const
{
    return mHighlightsWord;
}

void RoomExtra::setHighlightsWord(const QStringList &newHighlightsWord)
{
    mHighlightsWord = newHighlightsWord;
}

TeamInfo RoomExtra::teamInfo() const
{
    return mTeamInfo;
}

void RoomExtra::setTeamInfo(const TeamInfo &newTeamInfo)
{
    mTeamInfo = newTeamInfo;
}

bool RoomExtra::isDiscussionRoom() const
{
    return !mParentRid.isEmpty();
}

QByteArray RoomExtra::parentRid() const
{
    return mParentRid;
}

void RoomExtra::setParentRid(const QByteArray &parentRid)
{
    mParentRid = parentRid;
}

void RoomExtra::setMutedUsers(const QStringList &mutedUsers)
{
    mMutedUsers = mutedUsers;
}

qint64 RoomExtra::lastOpenedAt() const
{
    return mLastOpenedAt;
}

void RoomExtra::setLastOpenedAt(qint64 newLastOpenedAt)
{
    qCDebug(RUQOLA_MEMORY_MANAGEMENT_LOG) << "newLastOpenedAt " << newLastOpenedAt;
    mLastOpenedAt = newLastOpenedAt;
}

bool RoomExtra::canCleanHistory() const
{
    if (mLastOpenedAt == -1) {
        return false;
    }
    if (mLastOpenedAt - QDateTime::currentDateTime().addSecs(-3600).toSecsSinceEpoch() < 0) {
        qCDebug(RUQOLA_MEMORY_MANAGEMENT_LOG) << "name clean up room ";
        return true;
    }
    return false;
}
