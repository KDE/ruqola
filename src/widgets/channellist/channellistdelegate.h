/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "utils.h"
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
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

private:
    [[nodiscard]] QString makeUnreadText(const QModelIndex &index) const;

    RocketChatAccount *mRocketChatAccount = nullptr;
    AvatarCacheManager *const mAvatarCacheManager;
};
