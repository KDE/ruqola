/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QItemDelegate>

class AvatarCacheManager;
class RocketChatAccount;
class UserCompletionDelegate : public QItemDelegate
{
public:
    explicit UserCompletionDelegate(QObject *parent = nullptr);
    ~UserCompletionDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount);

    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    AvatarCacheManager *const mAvatarCacheManager;
};
