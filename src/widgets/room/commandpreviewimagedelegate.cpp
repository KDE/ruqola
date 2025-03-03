/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandpreviewimagedelegate.h"
#include "model/previewcommandmodel.h"
#include <QPainter>
CommandPreviewImageDelegate::CommandPreviewImageDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

CommandPreviewImageDelegate::~CommandPreviewImageDelegate() = default;

void CommandPreviewImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPixmap pixmap = index.data(static_cast<int>(PreviewCommandModel::PreviewCommandRoles::Image)).value<QPixmap>();
    if (!pixmap.isNull()) {
        const QRect rect = option.rect;
        painter->drawPixmap(rect, pixmap.scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        painter->drawText(option.rect, index.data(Qt::UserRole).toString());
    }
};

QSize CommandPreviewImageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPixmap pixmap = index.data(static_cast<int>(PreviewCommandModel::PreviewCommandRoles::Image)).value<QPixmap>();
    if (!pixmap.isNull()) {
        return pixmap.size();
    }
    return QSize(60, 60);
}

#include "moc_commandpreviewimagedelegate.cpp"
