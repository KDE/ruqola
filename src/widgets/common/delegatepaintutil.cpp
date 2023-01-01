/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "delegatepaintutil.h"
#include <QPainter>

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
