/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QItemDelegate>

class CommandCompletionDelegate : public QItemDelegate
{
public:
    explicit CommandCompletionDelegate(QObject *parent = nullptr);
    ~CommandCompletionDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

