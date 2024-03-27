/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomextra.h"
#include "ruqola_room_memory_debug.h"

RoomExtra::RoomExtra()
    : QSharedData()
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

void RoomExtra::setMutedUsers(const QStringList &mutedUsers)
{
    mMutedUsers = mutedUsers;
}
