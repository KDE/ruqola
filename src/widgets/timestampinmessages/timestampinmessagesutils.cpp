/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesutils.h"

QDebug operator<<(QDebug d, const TimeStampInMessagesUtils::TimeStampInfo &t)
{
    d.space() << "format:" << t.format;
    d.space() << "time:" << t.time;
    d.space() << "date:" << t.date;
    d.space() << "timeZone:" << t.timeZone;
    return d;
}

QString TimeStampInMessagesUtils::generateTimeStampStr(const TimeStampInfo &info)
{
    // TODO
    return {};
}
