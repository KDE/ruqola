/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselection.h"
#include "libruqolawidgets_private_export.h"
#include "misc/messagelistdelegatebase.h"
class QListView;

class LIBRUQOLAWIDGETS_TESTS_EXPORT NotificationHistoryDelegate : public MessageListDelegateBase
{
    Q_OBJECT
public:
    struct LIBRUQOLAWIDGETS_TESTS_EXPORT RoomAccount {
        QString channelName;
        QString accountName;
        [[nodiscard]] bool operator==(const RoomAccount &other) const
        {
            return (channelName == other.channelName) && (accountName == other.accountName);
        }
        [[nodiscard]] bool operator!=(const RoomAccount &other) const
        {
            return !operator==(other);
        }
    };
    explicit NotificationHistoryDelegate(QListView *view, QObject *parent = nullptr);
    ~NotificationHistoryDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    [[nodiscard]] bool helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    [[nodiscard]] bool mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

    [[nodiscard]] bool maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index);

protected:
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    [[nodiscard]] RocketChatAccount *rocketChatAccount(const QModelIndex &index) const override;

private:
    [[nodiscard]] QTextDocument *documentForModelIndex(const QModelIndex &index, int width) const override;

    struct Layout {
        // Sender
        QString senderText;
        QFont senderFont;
        QRectF senderRect;

        // Avatar pixmap
        QPixmap avatarPixmap;
        QPointF avatarPos;

        // Text message
        QRect textRect;
        qreal baseLine; // used to draw sender/timestamp

        QString timeStampText;
        QPoint timeStampPos;

        bool sameAccountRoomAsPreviousMessage = false;
    };
    LIBRUQOLAWIDGETS_NO_EXPORT void drawAccountRoomInfo(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT NotificationHistoryDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QPoint adaptMousePosition(const QPoint &pos, QRect textRect, const QStyleOptionViewItem &option);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString cacheIdentifier(const QModelIndex &index) const;
};
