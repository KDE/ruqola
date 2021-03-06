/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "common/delegatepaintutil.h"
#include "model/commandsmodel.h"

#include <QPainter>

CommandCompletionDelegate::CommandCompletionDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

CommandCompletionDelegate::~CommandCompletionDelegate()
{
}

void CommandCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // command <parameter> description at end
    drawBackground(painter, option, index);

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    const int margin = DelegatePaintUtil::margin();
    const QFont oldFont = painter->font();
    const QFontMetrics commandFontMetrics(painter->font());
    const QString commandText = index.data(CommandsModel::CommandName).toString();
    const int commandWidth = commandFontMetrics.horizontalAdvance(commandText);

    const int defaultCharHeight = option.rect.y() + commandFontMetrics.ascent();

    painter->drawText(margin, defaultCharHeight, commandText);

    QFont italicFont = oldFont;
    italicFont.setItalic(true);
    painter->setFont(italicFont);

    const QPen oldPen = painter->pen();
    QColor col = painter->pen().color();
    col.setAlpha(128);
    painter->setPen(col);

    const QString parameter = index.data(CommandsModel::TranslatedParams).toString();
    const int xText = option.rect.x() + 3 * margin + commandWidth;
    painter->drawText(xText, defaultCharHeight, parameter);

    painter->setFont(oldFont);

    const QString description = index.data(CommandsModel::TranslatedDescription).toString();
    const int descriptionWidth = commandFontMetrics.horizontalAdvance(description);

    painter->drawText(option.rect.width() - descriptionWidth - margin, defaultCharHeight, description);

    painter->setPen(oldPen);
}
