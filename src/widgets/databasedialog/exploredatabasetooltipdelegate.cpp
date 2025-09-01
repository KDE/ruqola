/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasetooltipdelegate.h"
#include <QHelpEvent>
#include <QToolTip>

ExploreDatabaseTooltipDelegate::ExploreDatabaseTooltipDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

ExploreDatabaseTooltipDelegate::~ExploreDatabaseTooltipDelegate() = default;

bool ExploreDatabaseTooltipDelegate::helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (!event || !view)
        return false;

    if (event->type() == QEvent::ToolTip) {
        const QString tooltip = index.data(Qt::DisplayRole).toString();
        QToolTip::showText(event->globalPos(), tooltip);
        return true;
    }
    return QStyledItemDelegate::helpEvent(event, view, option, index);
}

#include "moc_exploredatabasetooltipdelegate.cpp"
