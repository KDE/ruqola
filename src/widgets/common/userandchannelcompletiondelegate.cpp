/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userandchannelcompletiondelegate.h"
#include "common/delegatepaintutil.h"
#include "model/inputcompletermodel.h"

#include <QFontMetricsF>
#include <QPainter>

UserAndChannelCompletionDelegate::UserAndChannelCompletionDelegate(QObject *parent)
    : QItemDelegate{parent}
{
}

UserAndChannelCompletionDelegate::~UserAndChannelCompletionDelegate() = default;

void UserAndChannelCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] icon ? status name (username) (description if necessary)
    drawBackground(painter, option, index);

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    const int margin = DelegatePaintUtil::margin();
    const QFont oldFont = painter->font();

    QFont boldFont = oldFont;
    boldFont.setBold(true);
    painter->setFont(boldFont);

    int xPos = -1;
    const QIcon icon = index.data(InputCompleterModel::IconStatus).value<QIcon>();
    if (!icon.isNull()) {
        const QRect displayRect(margin, option.rect.y(), option.rect.height(), option.rect.height());
        drawDecoration(painter, option, displayRect, icon.pixmap(option.rect.height(), option.rect.height()));
        xPos = margin + option.rect.height();
    }

    QFontMetrics fontMetrics(boldFont);
    const QString name = index.data(InputCompleterModel::DisplayName).toString();
    const QString userName = index.data(InputCompleterModel::UserName).toString();
    int nameWidth = -1;
    const int defaultCharHeight = option.rect.y() + fontMetrics.ascent();
    if (name.isEmpty()) {
        nameWidth = fontMetrics.horizontalAdvance(userName);
        painter->drawText(xPos + margin, defaultCharHeight, userName);
        xPos += nameWidth;
    } else {
        nameWidth = fontMetrics.horizontalAdvance(name);
        painter->drawText(xPos + margin, defaultCharHeight, name);
        xPos += nameWidth;
        if (!userName.isEmpty()) {
            painter->setFont(oldFont);
            fontMetrics = QFontMetrics(oldFont);
            nameWidth = fontMetrics.horizontalAdvance(userName);
            painter->drawText(xPos + margin * 2, defaultCharHeight, userName);
            xPos += nameWidth;
        }
    }

    const QString description = index.data(InputCompleterModel::Description).toString();
    if (!description.isEmpty()) {
        QFont italicFont = oldFont;
        italicFont.setItalic(true);
        painter->setFont(italicFont);

        painter->drawText(xPos + margin * 2, defaultCharHeight, description);
    }
    painter->setFont(oldFont);
}
