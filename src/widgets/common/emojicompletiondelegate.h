/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QItemDelegate>

class EmojiCompletionDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit EmojiCompletionDelegate(QObject *parent = nullptr);
    ~EmojiCompletionDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    const QFont mEmojiFont;
};
