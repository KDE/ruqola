/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "reaction.h"
#include "emoticons/emojimanager.h"
#include <KLocalizedString>
Reaction::Reaction()
{
}

QString Reaction::convertedReactionName() const
{
    return mCacheConvertedReactionName;
}

QString Reaction::convertedUsersNameAtToolTip() const
{
    if (mUserNames.isEmpty()) {
        return QString();
    } else if (mUserNames.count() == 1) {
        return i18n("%1 reacted with %2", mUserNames[0], mReactionName);
    } else {
        QString notificationStr;
        for (int i = 0, total = mUserNames.count(); i < total; ++i) {
            const QString user = mUserNames.at(i);
            if (i == 0) {
                notificationStr = user;
            } else if (i < (total - 1)) {
                notificationStr = i18n("%1, %2", notificationStr, user);
            } else {
                notificationStr = i18n("%1 and %2", notificationStr, user);
            }
        }
        return i18n("%1 reacted with %2", notificationStr, mReactionName);
    }
}

bool Reaction::isAnimatedImage() const
{
    return mIsAnimatedImage;
}

void Reaction::setIsAnimatedImage(bool isAnimatedImage)
{
    mIsAnimatedImage = isAnimatedImage;
}

QString Reaction::reactionName() const
{
    return mReactionName;
}

void Reaction::setReactionName(const QString &reactionName, EmojiManager *emojiManager)
{
    if (mReactionName != reactionName) {
        mReactionName = reactionName;
        if (emojiManager) {
            mCacheConvertedReactionName = emojiManager->replaceEmojiIdentifier(mReactionName, true);
            mIsAnimatedImage = emojiManager->isAnimatedImage(mReactionName);
        }
    }
}

QStringList Reaction::userNames() const
{
    return mUserNames;
}

void Reaction::setUserNames(const QStringList &userNames)
{
    mUserNames = userNames;
}

int Reaction::count() const
{
    return mUserNames.count();
}

bool Reaction::operator ==(const Reaction &other) const
{
    return (mUserNames == other.userNames()) && (mReactionName == other.reactionName());
}

QDebug operator <<(QDebug d, const Reaction &t)
{
    d << "ReactionName " << t.reactionName();
    d << "UserNames " << t.userNames();
    return d;
}
