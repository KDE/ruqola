/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "utils.h"
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
    Q_REQUIRED_RESULT QPixmap makeAvatarPixmap(const QString &identifier, Utils::AvatarType type, const QWidget *widget, int maxHeight) const;
    AvatarCacheManager *const mAvatarCacheManager;
};
