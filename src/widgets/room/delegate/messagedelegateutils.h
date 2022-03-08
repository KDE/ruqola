/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QString>
#include <QTextDocument>
#include <memory>
namespace MessageDelegateUtils
{
std::unique_ptr<QTextDocument> createTextDocument(bool useItalic, const QString &text, int width);

Q_REQUIRED_RESULT bool generateToolTip(const QTextDocument *doc, const QPoint &pos, QString &formattedTooltip);
}

