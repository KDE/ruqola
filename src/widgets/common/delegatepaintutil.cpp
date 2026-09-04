/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "delegatepaintutil.h"
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionViewItem>

void DelegatePaintUtil::setTextPen(QPainter *painter, const QStyleOptionViewItem &option)
{
    QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled) ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(option.state & QStyle::State_Active)) {
        cg = QPalette::Inactive;
    }
    painter->setPen(option.palette.color(cg, (option.state & QStyle::State_Selected) ? QPalette::HighlightedText : QPalette::Text));
}

void DelegatePaintUtil::drawLighterText(QPainter *painter, const QString &text, QPoint pos)
{
    const QPen oldPen = painter->pen();
    QColor col = painter->pen().color();
    col.setAlpha(128);
    painter->setPen(col);
    painter->drawText(pos, text);
    painter->setPen(oldPen);
}

int DelegatePaintUtil::margin()
{
    return 8;
}

void DelegatePaintUtil::createClipRoundedRectangle(QPainter *painter, const QRectF &rect, const QPixmap &pix, qreal radius)
{
    QPainterPath path;
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    path.addRoundedRect(rect, radius, radius);
    painter->setClipPath(path);
    // Draw into the very same rect as the clip path, otherwise the corners of the
    // pixmap fall outside the rounded path and stay square.
    painter->drawPixmap(rect, pix, QRectF(pix.rect()));
    painter->restore();
}
