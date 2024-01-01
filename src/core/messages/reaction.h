/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] QString reactionName() const;
    void setReactionName(const QString &reactionName, EmojiManager *emojiManager = nullptr);

    [[nodiscard]] QStringList userNames() const;
    void setUserNames(const QStringList &userNames);

    [[nodiscard]] int count() const;

    [[nodiscard]] bool operator==(const Reaction &other) const;

    [[nodiscard]] QString convertedReactionName() const;
    [[nodiscard]] QString convertedUsersNameAtToolTip() const;

    [[nodiscard]] bool isAnimatedImage() const;
    void setIsAnimatedImage(bool isAnimatedImage);

private:
    QString mReactionName;
    QString mCacheConvertedReactionName;
    QStringList mUserNames;
    bool mIsAnimatedImage = false;
};
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(Reaction)
#endif
Q_DECLARE_TYPEINFO(Reaction, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Reaction &t);
