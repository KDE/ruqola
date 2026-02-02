/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesutils.h"
#include "ruqola_debug.h"
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

QString TimeStampInMessagesUtils::convertFormatTypeToString(TimeStampInMessagesUtils::FormatType type)
{
    switch (type) {
    case TimeStampInMessagesUtils::FormatType::ShortTime:
        return u"t"_s;
    case TimeStampInMessagesUtils::FormatType::LongTime:
        return u"T"_s;
    case TimeStampInMessagesUtils::FormatType::ShortDate:
        return u"d"_s;
    case TimeStampInMessagesUtils::FormatType::LongDate:
        return u"D"_s;
    case TimeStampInMessagesUtils::FormatType::FullDateTime:
        return u"f"_s;
    case TimeStampInMessagesUtils::FormatType::LongFullDateTime:
        return u"F"_s;
    case TimeStampInMessagesUtils::FormatType::RelativeTime:
        return u"R"_s;
    case TimeStampInMessagesUtils::FormatType::Unknown: {
        qCWarning(RUQOLA_LOG) << "Invalid format it's a bug";
        return {};
    }
    }
    return {};
}

TimeStampInMessagesUtils::FormatType TimeStampInMessagesUtils::convertStringToFormatType(const QString &str)
{
    if (str == u't') {
        return TimeStampInMessagesUtils::FormatType::ShortTime;
    } else if (str == u'T') {
        return TimeStampInMessagesUtils::FormatType::LongTime;
    } else if (str == u'd') {
        return TimeStampInMessagesUtils::FormatType::ShortDate;
    } else if (str == u'D') {
        return TimeStampInMessagesUtils::FormatType::LongDate;
    } else if (str == u'f') {
        return TimeStampInMessagesUtils::FormatType::FullDateTime;
    } else if (str == u'F') {
        return TimeStampInMessagesUtils::FormatType::LongFullDateTime;
    } else if (str == u'R') {
        return TimeStampInMessagesUtils::FormatType::RelativeTime;
    } else {
        qCWarning(RUQOLA_LOG) << "Invalid format it's a bug";
        return TimeStampInMessagesUtils::FormatType::Unknown;
    }
}

static QString generateNumber(int value, int pad)
{
    return u"%1"_s.arg(value, pad, 10, u'0');
}

QString TimeStampInMessagesUtils::generateTimeStampStr(const TimeStampInfo &info)
{
    if (info.format == FormatType::Unknown) {
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
    return u"<t:%1:%2>"_s.arg(result, convertFormatTypeToString(info.format));
}

bool TimeStampInMessagesUtils::TimeStampInfo::isValid() const
{
    return format != FormatType::Unknown;
}

#include "moc_timestampinmessagesutils.cpp"
