/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandpreviewimagedelegate.h"

CommandPreviewImageDelegate::CommandPreviewImageDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

CommandPreviewImageDelegate::~CommandPreviewImageDelegate() = default;

void CommandPreviewImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Draw image !
}

QSize CommandPreviewImageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

#include "moc_commandpreviewimagedelegate.cpp"
