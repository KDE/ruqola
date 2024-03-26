/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include "teams/teaminfo.h"

#include <QSharedData>
#include <QStringList>
class LIBRUQOLACORE_EXPORT RoomExtra : public QSharedData
{
public:
    RoomExtra();
    ~RoomExtra();

private:
    // muted - collection of muted users by its usernames
    QStringList mMutedUsers;

    QStringList mIgnoredUsers;

    QStringList mHighlightsWord;

    // Autotranslate
    QString mAutotranslateLanguage;

    TeamInfo mTeamInfo;
};
