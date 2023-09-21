/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "reaction.h"
#include <QVector>
class EmojiManager;
class LIBRUQOLACORE_EXPORT Reactions
{
public:
    Reactions();

    void setReactions(const QVector<Reaction> &reactions);
    [[nodiscard]] QVector<Reaction> reactions() const;

    void parseReactions(const QJsonObject &array, EmojiManager *emojiManager = nullptr);

    [[nodiscard]] bool operator==(const Reactions &other) const;

    [[nodiscard]] static QJsonObject serialize(const Reactions &reactions);
    [[nodiscard]] static Reactions deserialize(const QJsonObject &o, EmojiManager *emojiManager = nullptr);

    [[nodiscard]] bool isEmpty() const;

private:
    QVector<Reaction> mReactions;
};
Q_DECLARE_METATYPE(Reactions)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Reactions &t);
