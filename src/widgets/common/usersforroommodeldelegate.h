/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QItemDelegate>

class AvatarCacheManager;
class RocketChatAccount;
class UsersForRoomModelDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit UsersForRoomModelDelegate(QObject *parent = nullptr);
    ~UsersForRoomModelDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    [[nodiscard]] bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    AvatarCacheManager *const mAvatarCacheManager;
};
