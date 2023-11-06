/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QItemDelegate>

class UserAndChannelCompletionDelegate : public QItemDelegate
{
public:
    explicit UserAndChannelCompletionDelegate(QObject *parent = nullptr);
    ~UserAndChannelCompletionDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
