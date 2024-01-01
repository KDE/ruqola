/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandcompletiondelegate.h"

#include "common/delegatepaintutil.h"
#include "model/commandsmodel.h"

#include <QPainter>

CommandCompletionDelegate::CommandCompletionDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

CommandCompletionDelegate::~CommandCompletionDelegate() = default;

void CommandCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // command <parameter> description at end
    drawBackground(painter, option, index);

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    const int margin = DelegatePaintUtil::margin();
    const QFont oldFont = painter->font();
    const QFontMetrics commandFontMetrics(oldFont);
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
