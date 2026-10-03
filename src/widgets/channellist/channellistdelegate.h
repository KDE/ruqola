/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ownuser/ownuserpreferences.h"
#include <QItemDelegate>

class RocketChatAccount;
class AvatarCacheManager;
class ChannelListDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ChannelListDelegate(QObject *parent = nullptr);
    ~ChannelListDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

    void setListDisplay(OwnUserPreferences::RoomListDisplay display);

    [[nodiscard]] bool helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    struct Layout {
        QString unreadText;
        // The badge actually painted for unreadText: right-aligned on the text extent, widened to
        // a circle and vertically centered in the row. Used for painting *and* tooltip hit-testing.
        QRect mentionRect;
        bool isHeader = false;
    };

    [[nodiscard]] ChannelListDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    [[nodiscard]] static QString makeUnreadText(const QModelIndex &index);
    void clearAvatarCache();

    OwnUserPreferences::RoomListDisplay mRoomListDisplay = OwnUserPreferences::RoomListDisplay::Unknown;
    RocketChatAccount *mRocketChatAccount = nullptr;
    AvatarCacheManager *const mAvatarCacheManager;
};
