/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QItemDelegate>

class EmojiCustomDelegate : public QItemDelegate
{
public:
    explicit EmojiCustomDelegate(QObject *parent = nullptr);
    ~EmojiCustomDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    const QFont mEmojiFont;
};
