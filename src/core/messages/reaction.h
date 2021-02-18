/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
