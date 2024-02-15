/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QItemDelegate>
class AvatarCacheManager;
class RocketChatAccount;
class ForwardMessageUserAndChannelCompletionDelegate : public QItemDelegate
{
public:
    explicit ForwardMessageUserAndChannelCompletionDelegate(QObject *parent = nullptr);
    ~ForwardMessageUserAndChannelCompletionDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    AvatarCacheManager *const mAvatarCacheManager;
};
