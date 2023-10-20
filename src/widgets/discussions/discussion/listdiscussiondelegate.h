/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "misc/messagelistdelegatebase.h"
class QTextDocument;
class RocketChatAccount;
class AvatarCacheManager;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ListDiscussionDelegate : public MessageListDelegateBase
{
    Q_OBJECT
public:
    explicit ListDiscussionDelegate(QListView *view, RocketChatAccount *account, QObject *parent = nullptr);
    ~ListDiscussionDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    [[nodiscard]] bool mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

Q_SIGNALS:
    void openDiscussion(const QString &discussionRoomId);

protected:
    [[nodiscard]] RocketChatAccount *rocketChatAccount(const QModelIndex &index) const override;

private:
    struct Layout {
        // Sender
        QString senderText;
        QFont senderFont;
        QRectF senderRect;

        // TimeStamp
        QString lastMessageTimeText;
        qreal lastMessageTimeY = 0;

        // Number of message
        int numberOfMessages = 0;

        // OpenDiscussionText
        QString openDiscussionText;
        qreal openDiscussionTextY = 0;
        qreal openDiscussionTextHeight = 0;

        // Avatar pixmap
        QPixmap avatarPixmap;
        QPointF avatarPos;

        // Text message
        QRect textRect;
        qreal baseLine; // used to draw sender/timestamp
    };
    [[nodiscard]] ListDiscussionDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    [[nodiscard]] QTextDocument *documentForModelIndex(const QModelIndex &index, int width) const override;
    [[nodiscard]] QPixmap makeAvatarPixmap(const QWidget *widget, const QModelIndex &index, int maxHeight) const;
    [[nodiscard]] QPoint adaptMousePosition(const QPoint &pos, QRect textRect, const QStyleOptionViewItem &option);
    [[nodiscard]] QString cacheIdentifier(const QModelIndex &index) const;

    RocketChatAccount *const mRocketChatAccount;
    AvatarCacheManager *const mAvatarCacheManager;
};
