/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
class EmojiManager;
class LIBRUQOLACORE_EXPORT Reaction
{
    Q_GADGET
public:
    Reaction();

    Q_REQUIRED_RESULT QString reactionName() const;
    void setReactionName(const QString &reactionName, EmojiManager *emojiManager = nullptr);

    Q_REQUIRED_RESULT QStringList userNames() const;
    void setUserNames(const QStringList &userNames);

    Q_REQUIRED_RESULT int count() const;

    Q_REQUIRED_RESULT bool operator==(const Reaction &other) const;

    Q_REQUIRED_RESULT QString convertedReactionName() const;
    Q_REQUIRED_RESULT QString convertedUsersNameAtToolTip() const;

    Q_REQUIRED_RESULT bool isAnimatedImage() const;
    void setIsAnimatedImage(bool isAnimatedImage);

private:
    QString mReactionName;
    QString mCacheConvertedReactionName;
    QStringList mUserNames;
    bool mIsAnimatedImage = false;
};
Q_DECLARE_METATYPE(Reaction)
Q_DECLARE_TYPEINFO(Reaction, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Reaction &t);
