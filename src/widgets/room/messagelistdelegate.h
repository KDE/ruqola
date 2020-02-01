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

#include "libruqolawidgets_private_export.h"

#include <QFont>
#include <QItemDelegate>
#include <QScopedPointer>
#include "messages/reaction.h"

class RocketChatAccount;
class Message;
class MessageDelegateHelperBase;
class MessageDelegateHelperText;
class MessageDelegateHelperImage;
class MessageDelegateHelperFile;

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageListDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit MessageListDelegate(QObject *parent = nullptr);
    ~MessageListDelegate();

    void setRocketChatAccount(RocketChatAccount *rcAccount);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    void drawDate(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option) const;

private:
    void drawReactions(QPainter *painter, const QModelIndex &index, const QRect &usableRect, const QStyleOptionViewItem &option) const;
    QPixmap makeAvatarPixmap(const QModelIndex &index, int maxHeight) const;

    struct ReactionLayout {
        QRectF reactionRect;
        QRectF countRect;
        QString emojiString;
        QString countStr;
        qreal emojiOffset;
        Reaction reaction;
        bool useEmojiFont;
    };
    QVector<ReactionLayout> layoutReactions(const QVector<Reaction> &reactions, const qreal messageX, const QStyleOptionViewItem &option) const;

    struct Layout {
        // Sender
        QString senderText;
        QFont senderFont;
        QRectF senderRect;
        qreal ascent;

        // Avatar pixmap
        QPixmap avatarPixmap;
        qreal avatarX;

        // Timestamp
        QString timeStampText;
        QSize timeSize;

        QRect usableRect; // rect for everything except the date header (at the top) and the sender (on the left)

        // Text message
        QRect textRect;

        // Attachments
        QRect attachmentsRect;

        // Reactions
        qreal reactionsHeight;
    };
    Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    /// @note Ownership is not transferred
    MessageDelegateHelperBase *attachmentsHelper(const Message *message) const;

    friend class MessageListDelegateTest;

    QFont mEmojiFont;
    RocketChatAccount *mRocketChatAccount = nullptr;

    QScopedPointer<MessageDelegateHelperText> mHelperText;
    QScopedPointer<MessageDelegateHelperImage> mHelperImage;
    QScopedPointer<MessageDelegateHelperFile> mHelperFile;
};

#endif // MESSAGELISTDELEGATE_H
