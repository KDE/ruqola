/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesutils.h"
using namespace Qt::Literals::StringLiterals;
//./apps/meteor/client/lib/utils/timestamp/conversion.ts
QDebug operator<<(QDebug d, const TimeStampInMessagesUtils::TimeStampInfo &t)
{
    d.space() << "format:" << t.format;
    d.space() << "time:" << t.time;
    d.space() << "date:" << t.date;
    d.space() << "timeZone:" << t.timeZone;
    return d;
}

static QString generateNumber(int value, int pad)
{
    return u"%1"_s.arg(value, pad, 10, u'0');
}

QString TimeStampInMessagesUtils::generateTimeStampStr(const TimeStampInfo &info)
{
    if (info.format.isEmpty()) {
        return {};
    }
    const int year = info.date.year();
    const int month = info.date.month();
    const int day = info.date.day();

    const int hours = info.time.hour();
    const int minutes = info.time.minute();
    const int secondes = info.time.second();
    const int msec = info.time.msec();
    const QString result = u"%1-%2-%3T%4:%5:%6.%7%8"_s.arg(generateNumber(year, 2),
                                                           generateNumber(month, 2),
                                                           generateNumber(day, 2),
                                                           generateNumber(hours, 2),
                                                           generateNumber(minutes, 2),
                                                           generateNumber(secondes, 2),
                                                           generateNumber(msec, 3),
                                                           info.timeZone);

#if 0
    const month = String(date.getMonth() + 1).padStart(2, '0');
    const day = String(date.getDate()).padStart(2, '0');
    const hours = String(date.getHours()).padStart(2, '0');
    const minutes = String(date.getMinutes()).padStart(2, '0');
    const seconds = String(date.getSeconds()).padStart(2, '0');
    const ms = String(date.getMilliseconds()).padStart(3, '0');
    return `${year}-${month}-${day}T${hours}:${minutes}:${seconds}.${ms}${offset}`;
#endif

    // TODO
    return u"<t:%1:%2>"_s.arg(result, info.format);
}

bool TimeStampInMessagesUtils::TimeStampInfo::isValid() const
{
    return !format.isEmpty();
}
