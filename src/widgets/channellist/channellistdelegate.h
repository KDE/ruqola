/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

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
        QSize unreadSize;
        QRect unreadRect;
        bool isHeader = false;
    };

    [[nodiscard]] ChannelListDelegate::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    [[nodiscard]] QString makeUnreadText(const QModelIndex &index) const;
    void clearAvatarCache();

    OwnUserPreferences::RoomListDisplay mRoomListDisplay = OwnUserPreferences::RoomListDisplay::Unknown;
    RocketChatAccount *mRocketChatAccount = nullptr;
    AvatarCacheManager *const mAvatarCacheManager;
};
