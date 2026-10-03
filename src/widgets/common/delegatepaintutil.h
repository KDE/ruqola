/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QString>
class QPainter;
class QPoint;
class QRectF;
class QPixmap;
class QStyleOptionViewItem;

namespace DelegatePaintUtil
{
/**
 * Sets @p painter's pen to the text color matching @p option's state, as
 * QItemDelegate::drawDisplay() does: QPalette::HighlightedText on a selected row and
 * QPalette::Text otherwise, in the color group matching the enabled/active state.
 * Delegates painting their text with QPainter::drawText() must call it, otherwise the text
 * keeps the view foreground color and stays unreadable over the selection background.
 */
void setTextPen(QPainter *painter, const QStyleOptionViewItem &option);
void drawLighterText(QPainter *painter, const QString &text, QPoint pos);
[[nodiscard]] int margin();
/**
 * Draws @p pix inside @p rect, clipped to a rounded rectangle.
 * @p rect is in logical (device independent) coordinates: use
 * QRectF(pos, pix.deviceIndependentSize()) to draw the pixmap at its natural size.
 */
void createClipRoundedRectangle(QPainter *painter, const QRectF &rect, const QPixmap &pix, qreal radius = 5);
}
