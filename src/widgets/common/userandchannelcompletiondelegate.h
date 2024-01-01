/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QItemDelegate>
class AvatarCacheManager;
class RocketChatAccount;
class UserAndChannelCompletionDelegate : public QItemDelegate
{
public:
    explicit UserAndChannelCompletionDelegate(QObject *parent = nullptr);
    ~UserAndChannelCompletionDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

private:
    AvatarCacheManager *const mAvatarCacheManager;
};
