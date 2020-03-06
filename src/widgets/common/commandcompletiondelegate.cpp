/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "commandcompletiondelegate.h"

#include "model/commandsmodel.h"

#include <QPainter>

CommandCompletionDelegate::CommandCompletionDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

void CommandCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // command <parameter> description at end
    drawBackground(painter, option, index);
    const int margin = 8;
    QFont font = painter->font();
    QFontMetrics commandFontMetrics(painter->font());
    const QString commandText = index.data(CommandsModel::CommandName).toString();
    const int commandWidth = commandFontMetrics.horizontalAdvance(commandText);
    painter->drawText(margin, option.rect.y(), commandText);

    font.setItalic(true);


    const QString parameter = index.data(Qt::DisplayRole).toString();
    const int xText = option.rect.x() + margin + commandWidth;
    painter->drawText(xText, option.rect.y(), parameter);
    const QRect displayRect(xText, option.rect.y(),
                            option.rect.width() - xText,
                            option.rect.height());

    drawDisplay(painter, option, displayRect, parameter);
}
