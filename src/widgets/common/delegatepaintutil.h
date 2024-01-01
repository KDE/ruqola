/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QString>
class QPainter;
class QPoint;

namespace DelegatePaintUtil
{
void drawLighterText(QPainter *painter, const QString &text, QPoint pos);
[[nodiscard]] int margin();
}
