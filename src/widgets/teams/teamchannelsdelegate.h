/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QItemDelegate>
class AvatarCacheManager;
class RocketChatAccount;
class TeamChannelsDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit TeamChannelsDelegate(QObject *parent = nullptr);
    ~TeamChannelsDelegate() override;

    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    AvatarCacheManager *const mAvatarCacheManager;
};
