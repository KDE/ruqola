/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QItemDelegate>
class RocketChatAccount;
class AvatarCacheManager;
class SwitchChannelDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SwitchChannelDelegate(QObject *parent = nullptr);
    ~SwitchChannelDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

private:
    RocketChatAccount *mRocketChatAccount = nullptr;
    AvatarCacheManager *const mAvatarCacheManager;
};
