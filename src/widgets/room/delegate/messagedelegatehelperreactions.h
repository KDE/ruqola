/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/pixmapcache.h"
#include "runninganimatedimage.h"

#include "messages/reaction.h"
#include <QFont>
#include <QRectF>
#include <QSize>

#include <vector>

class QHelpEvent;
class QPainter;
class QRect;
class QModelIndex;
class QMouseEvent;
class QStyleOptionViewItem;
class Message;
class RocketChatAccount;

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperReactions
{
public:
    explicit MessageDelegateHelperReactions(RocketChatAccount *account);
    void draw(QPainter *painter, QRect reactionsRect, const QModelIndex &index, const QStyleOptionViewItem &option) const;
    [[nodiscard]] QSize sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const;
    [[nodiscard]] bool handleMouseEvent(QMouseEvent *mouseEvent, QRect reactionsRect, const QStyleOptionViewItem &option, const Message *message);
    [[nodiscard]] bool handleHelpEvent(QHelpEvent *helpEvent, QWidget *view, QRect reactionsRect, const QStyleOptionViewItem &option, const Message *message);

    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

private:
    Q_DISABLE_COPY(MessageDelegateHelperReactions)
    struct ReactionLayout {
        QRectF reactionRect;
        QRectF countRect;
        QString emojiString; // for unicode emojis
        QString emojiImagePath; // for custom emojis (pixmaps)
        QString countStr;
        qreal emojiOffset;
        Reaction reaction;
        bool useEmojiFont;
    };

    [[nodiscard]] std::vector<RunningAnimatedImage>::iterator findRunningAnimatedImage(const QModelIndex &index) const;
    void removeRunningAnimatedImage(const QModelIndex &index) const;
    [[nodiscard]] QVector<ReactionLayout> layoutReactions(const QVector<Reaction> &reactions, QRect reactionsRect, const QStyleOptionViewItem &option) const;
    const QFont mEmojiFont;
    mutable std::vector<RunningAnimatedImage> mRunningAnimatedImages; // not a hash or map, since QPersistentModelIndex changes value
    mutable PixmapCache mPixmapCache;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
