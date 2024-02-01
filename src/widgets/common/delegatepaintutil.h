/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QString>
class QPainter;
class QPointF;
class QPoint;
class QRectF;
class QPixmap;

namespace DelegatePaintUtil
{
void drawLighterText(QPainter *painter, const QString &text, QPoint pos);
[[nodiscard]] int margin();
void createClipRoundedRectangle(QPainter *painter, const QRectF &rect, QPointF pos, const QPixmap &pix);
}
