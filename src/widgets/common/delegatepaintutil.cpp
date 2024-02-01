/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "delegatepaintutil.h"
#include <QPainter>
#include <QPainterPath>

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

void DelegatePaintUtil::createClipRoundedRectangle(QPainter *painter, const QRectF &rect, QPointF pos, const QPixmap &pix)
{
    QPainterPath path;
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    path.addRoundedRect(rect, 5, 5);
    painter->setClipPath(path);
    painter->drawPixmap(pos, pix);
    painter->restore();
}
