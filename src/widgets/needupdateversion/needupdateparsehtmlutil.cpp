/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "needupdateparsehtmlutil.h"
#include <QRegularExpression>
QString NeedUpdateParseHtmlUtil::extractDate(const QString &dataHtml)
{
    if (dataHtml.isEmpty()) {
        return {};
    }
    static const QRegularExpression reg(QStringLiteral("<td align=\"right\">(\\d+\\-\\d\\d\\-\\d\\dd).*<"));
    // TODO
    return {};
}
