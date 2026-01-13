/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_export.h"
#include <QDate>
#include <QTime>
class QDebug;
namespace TimeStampInMessagesUtils
{
struct LIBRUQOLAWIDGETS_EXPORT TimeStampInfo {
    QString format;
    QDate date;
    QTime time;
    QString timeZone;
};
[[nodiscard]] LIBRUQOLAWIDGETS_EXPORT QString generateTimeStampStr(const TimeStampInfo &info);
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const TimeStampInMessagesUtils::TimeStampInfo &t);
