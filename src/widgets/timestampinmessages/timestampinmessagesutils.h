/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_export.h"
class QDebug;
namespace TimeStampInMessagesUtils
{
struct TimeStampInfo {
    QString format;
    QString date;
    QString time;
    QString timeZone;
};
[[nodiscard]] QString generateTimeStampStr(const TimeStampInfo &info);
// TODO allow to convert to string
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const TimeStampInMessagesUtils::TimeStampInfo &t);
