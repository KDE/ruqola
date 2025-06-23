/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "needupdateparsehtmlutil.h"
using namespace Qt::Literals::StringLiterals;

#include <QRegularExpression>
QString NeedUpdateParseHtmlUtil::extractDate(const QString &dataHtml)
{
    if (dataHtml.isEmpty()) {
        return {};
    }
    // By default generated html page will not changed in the future
    static const QRegularExpression reg(u"<td align=\"right\">(\\d+\\-\\d\\d\\-\\d\\d).*<"_s);

    const QRegularExpressionMatch match = reg.match(dataHtml);
    if (match.hasMatch()) {
        const QString matched = match.captured(1);
        return matched;
    }
    return {};
}
