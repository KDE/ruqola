/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QStyledItemDelegate>
class QListView;
class CommandPreviewImageDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CommandPreviewImageDelegate(QListView *view, QObject *parent = nullptr);
    ~CommandPreviewImageDelegate() override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QListView *const mView;
};
