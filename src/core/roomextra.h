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

    [[nodiscard]] QString autoTranslateLanguage() const;
    void setAutoTranslateLanguage(const QString &autotranslateLanguage);

    void setMutedUsers(const QStringList &mutedUsers);
    [[nodiscard]] QStringList mutedUsers() const;

    [[nodiscard]] QStringList ignoredUsers() const;
    void setIgnoredUsers(const QStringList &newIgnoredUsers);

    [[nodiscard]] QStringList highlightsWord() const;
    void setHighlightsWord(const QStringList &newHighlightsWord);

    [[nodiscard]] TeamInfo teamInfo() const;
    void setTeamInfo(const TeamInfo &newTeamInfo);

private:
    // muted - collection of muted users by its usernames
    QStringList mMutedUsers;

    QStringList mIgnoredUsers;

    QStringList mHighlightsWord;

    // Autotranslate
    QString mAutotranslateLanguage;

    TeamInfo mTeamInfo;
};
