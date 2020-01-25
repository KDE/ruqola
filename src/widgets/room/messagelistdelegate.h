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

#ifndef MESSAGELISTDELEGATE_H
#define MESSAGELISTDELEGATE_H

#include <QFont>
#include <QItemDelegate>
#include "messages/reaction.h"

class RocketChatAccount;
class Message;
class MessageDelegateHelperBase;
class MessageDelegateHelperText;
class MessageDelegateHelperImage;

class MessageListDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit MessageListDelegate(RocketChatAccount *rcAccount, QObject *parent = nullptr);
    ~MessageListDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    void drawReactions(QPainter *painter, const QModelIndex &index, const QRect &messageRect, const QStyleOptionViewItem &option) const;
    QPixmap makeAvatarPixmap(const QModelIndex &index, int maxHeight) const;

    struct ReactionLayout {
        QRectF reactionRect;
        QRectF countRect;
        QString emojiString;
        QString countStr;
        qreal emojiOffset;
        Reaction reaction;
    };
    QVector<ReactionLayout> layoutReactions(const QVector<Reaction> &reactions, const qreal messageX, const QStyleOptionViewItem &option) const;

    struct PixmapAndSenderLayout {
        QString senderText;
        QFont senderFont;
        QRectF senderRect;
        qreal ascent;
        QPixmap avatarPixmap;
        qreal avatarX;
    };
    PixmapAndSenderLayout layoutPixmapAndSender(const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const;
    MessageDelegateHelperBase *helper(const Message *message) const;

    QFont m_emojiFont;
    RocketChatAccount *m_rcAccount;

    MessageDelegateHelperText *m_helperText;
    MessageDelegateHelperImage *m_helperImage;
};

#endif // MESSAGELISTDELEGATE_H
