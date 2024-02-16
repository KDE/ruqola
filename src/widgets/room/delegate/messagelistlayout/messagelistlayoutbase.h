/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QFont>
#include <QList>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QString>
#include <QStyleOptionViewItem>

class RocketChatAccount;
class MessageListDelegate;
class Message;
class MessageListLayoutBase
{
public:
    explicit MessageListLayoutBase(MessageListDelegate *delegate);
    virtual ~MessageListLayoutBase();

    struct Layout {
        // Sender
        QString senderText;
        QFont senderFont;
        QRectF senderRect;

        // Avatar pixmap
        QPixmap avatarPixmap;
        QPointF avatarPos;

        // Roles icon
        QRect rolesIconRect;

        // Edited icon
        QRect editedIconRect;

        // Favorite icon
        QRect favoriteIconRect;

        // Pinned icon
        QRect pinIconRect;

        // Pinned icon
        QRect followingIconRect;

        // Translated icon
        QRect translatedIconRect;

        // Show Ignore Message icon
        QRect showIgnoredMessageIconRect;

        // add-reaction button and timestamp
        QRect addReactionRect;
        QString timeStampText;
        QPoint timeStampPos;
        QRect timeStampRect;

        QRect usableRect; // rect for everything except the date header (at the top) and the sender (on the left)

        // Text message
        QRect textRect;
        qreal baseLine; // used to draw sender/timestamp

        // Attachments
        QRect attachmentsRect;
        QList<QRect> attachmentsRectList;

        // Blocks
        QRect blocksRect;
        QList<QRect> blocksRectList;

        // MessageUrl preview
        QRect messageUrlsRect;
        QList<QRect> messageUrlsRectList;

        // Reactions
        qreal reactionsY = 0;
        qreal reactionsHeight = 0;

        // Replies
        qreal repliesY = 0;
        qreal repliesHeight = 0;

        // Discussions
        qreal discussionsHeight = 0;

        // Last See
        qreal displayLastSeenMessageY = 0;

        // showIgnoreMessage
        bool showIgnoreMessage = false;
        bool sameSenderAsPreviousMessage = false;
        bool messageIsFollowing = false;
    };

    [[nodiscard]] virtual MessageListLayoutBase::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;

    [[nodiscard]] virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;

    [[nodiscard]] RocketChatAccount *rocketChatAccount() const;
    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

protected:
    void generateSenderInfo(Layout &layout, const Message *message, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void generateAttachmentBlockAndUrlPreviewLayout(MessageListDelegate *delegate,
                                                    Layout &layout,
                                                    const Message *message,
                                                    int attachmentsY,
                                                    int textLeft,
                                                    int maxWidth,
                                                    const QStyleOptionViewItem &option,
                                                    const QModelIndex &index) const;
    [[nodiscard]] bool sameSenderAsPreviousMessage(const QModelIndex &index, const Message *message) const;
    [[nodiscard]] QString senderText(const Message *message) const;
    RocketChatAccount *mRocketChatAccount = nullptr;
    MessageListDelegate *mDelegate = nullptr;
};
