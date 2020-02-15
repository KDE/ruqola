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

#ifndef MESSAGEDELEGATEHELPERREACTIONS_H
#define MESSAGEDELEGATEHELPERREACTIONS_H

#include "libruqolawidgets_private_export.h"
#include "pixmapcache.h"

#include <QRectF>
#include <QSize>
#include <QFont>
#include "messages/reaction.h"

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
    void draw(QPainter *painter, const QRect &reactionsRect, const QModelIndex &index, const QStyleOptionViewItem &option) const;
    QSize sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const;
    bool handleMouseEvent(QMouseEvent *mouseEvent, const QRect &reactionsRect, const QStyleOptionViewItem &option, const Message *message);
    bool handleHelpEvent(QHelpEvent *helpEvent, QWidget *view, const QRect &reactionsRect, const QStyleOptionViewItem &option, const Message *message);

private:
    struct ReactionLayout {
        QRectF reactionRect;
        QRectF countRect;
        QString emojiString;    // for unicode emojis
        QString emojiImagePath; // for custom emojis (pixmaps)
        QString countStr;
        qreal emojiOffset;
        Reaction reaction;
        bool useEmojiFont;
    };

    QVector<ReactionLayout> layoutReactions(const QVector<Reaction> &reactions, const QRect &reactionsRect, const QStyleOptionViewItem &option) const;
    QFont mEmojiFont;
    mutable PixmapCache mPixmapCache;
};

#endif // MESSAGEDELEGATEHELPERREACTIONS_H
