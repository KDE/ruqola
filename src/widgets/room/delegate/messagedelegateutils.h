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
Q_REQUIRED_RESULT std::unique_ptr<QTextDocument> createTextDocument(bool useItalic, const QString &text, int width);

Q_REQUIRED_RESULT bool generateToolTip(const QTextDocument *doc, const QPoint &pos, QString &formattedTooltip);

void generateToolTip(const QString &toolTip, const QString &href, QString &formattedTooltip);

Q_REQUIRED_RESULT bool useItalicsForMessage(const QModelIndex &index);

Q_REQUIRED_RESULT bool pendingMessage(const QModelIndex &index);
}
