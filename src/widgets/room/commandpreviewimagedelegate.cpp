/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandpreviewimagedelegate.h"
#include "model/previewcommandmodel.h"
#include <QListView>
#include <QPainter>
CommandPreviewImageDelegate::CommandPreviewImageDelegate(QListView *view, QObject *parent)
    : QStyledItemDelegate{parent}
    , mView(view)
{
}

CommandPreviewImageDelegate::~CommandPreviewImageDelegate() = default;

void CommandPreviewImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const bool hasFocus = option.state & QStyle::State_HasFocus;

    QPixmap pixmap = index.data(static_cast<int>(PreviewCommandModel::PreviewCommandRoles::Image)).value<QPixmap>();
    if (!pixmap.isNull()) {
        const QRect rect = option.rect.adjusted(5, 5, -5, -5);
        painter->drawPixmap(rect, pixmap.scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        if (hasFocus) {
            painter->drawRect(option.rect.adjusted(0, 0, -1, -1));
        }
    } else {
        painter->drawText(option.rect, index.data(Qt::UserRole).toString());
    }
};

QSize CommandPreviewImageDelegate::sizeHint([[maybe_unused]] const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QPixmap pixmap = index.data(static_cast<int>(PreviewCommandModel::PreviewCommandRoles::Image)).value<QPixmap>();
    if (!pixmap.isNull()) {
        return pixmap.size();
    }
    return QSize(60, 60);
}

#include "moc_commandpreviewimagedelegate.cpp"
