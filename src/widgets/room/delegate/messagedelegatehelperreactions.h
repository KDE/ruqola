/*
   Copyright (c) 2020 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperReactions
{
public:
    MessageDelegateHelperReactions();
    void draw(QPainter *painter, QRect reactionsRect, const QModelIndex &index, const QStyleOptionViewItem &option) const;
    Q_REQUIRED_RESULT QSize sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const;
    Q_REQUIRED_RESULT bool handleMouseEvent(QMouseEvent *mouseEvent, QRect reactionsRect, const QStyleOptionViewItem &option, const Message *message);
    Q_REQUIRED_RESULT bool
    handleHelpEvent(QHelpEvent *helpEvent, QWidget *view, QRect reactionsRect, const QStyleOptionViewItem &option, const Message *message);

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
};

