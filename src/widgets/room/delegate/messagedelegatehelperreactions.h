/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/pixmapcache.h"

#include "messages/reaction.h"
#include <QFont>
#include <QRectF>
#include <QSize>

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
    Q_REQUIRED_RESULT QSize sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const;
    Q_REQUIRED_RESULT bool handleMouseEvent(QMouseEvent *mouseEvent, QRect reactionsRect, const QStyleOptionViewItem &option, const Message *message);
    Q_REQUIRED_RESULT bool
    handleHelpEvent(QHelpEvent *helpEvent, QWidget *view, QRect reactionsRect, const QStyleOptionViewItem &option, const Message *message);

    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

private:
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

    Q_REQUIRED_RESULT QVector<ReactionLayout>
    layoutReactions(const QVector<Reaction> &reactions, QRect reactionsRect, const QStyleOptionViewItem &option) const;
    const QFont mEmojiFont;
    mutable PixmapCache mPixmapCache;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
