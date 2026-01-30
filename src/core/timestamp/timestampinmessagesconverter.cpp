/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "timestampinmessagesconverter.h"
#include <QDateTime>
#include <QRegularExpression>
using namespace Qt::Literals::StringLiterals;

TimeStampInMessagesConverter::TimeStampInMessagesConverter() = default;

TimeStampInMessagesConverter::~TimeStampInMessagesConverter() = default;

QString TimeStampInMessagesConverter::regularExpressionStr() const
{
    return u"<t:([^>]*?)(?::([tTdDFfR]))?>"_s;
}

QString TimeStampInMessagesConverter::generateTimeStamp(const QString &str) const
{
    if (str.isEmpty()) {
        return {};
    }

    const static QRegularExpression reg(regularExpressionStr());

    QString newStr = str;
    QRegularExpressionMatchIterator roomIterator = reg.globalMatch(str);
    while (roomIterator.hasNext()) {
        const QRegularExpressionMatch match = roomIterator.next();
        const QStringView dateTime = match.capturedView(1);
        const QStringView format = match.capturedView(2);

        const QString result = convertTimeStamp(QDateTime::fromString(dateTime.toString(), u"yyyy-MM-ddThh:mm:ss.zzz"_s),
                                                TimeStampInMessagesUtils::convertStringToFormatType(format.toString()));
        // qDebug() << "dateTime " << dateTime << " format " << format << " result " << result;
        // qDebug() << "match.captured(0) " << match.captured(0) << " QDateTime::fromString(dateTime.toString()) " <<
        // QDateTime::fromString(dateTime.toString()); qDebug() << " QDateTime::fromString(dateTime.toString(), udd_s) " <<
        // QDateTime::fromString(dateTime.toString(), u"yyyy-MM-ddThh:mm:ss.zzz"_s);
        newStr.replace(match.captured(0), result);
    }
    return newStr;
}

QString TimeStampInMessagesConverter::convertTimeStamp(const QDateTime &dateTime, TimeStampInMessagesUtils::FormatType type) const
{
    QString dateTimeStr;
    switch (type) {
    case TimeStampInMessagesUtils::FormatType::FullDateTime:
        dateTimeStr = dateTime.toString(Qt::DateFormat::ISODate);
        break;
    case TimeStampInMessagesUtils::FormatType::LongFullDateTime:
        dateTimeStr = dateTime.toString(Qt::DateFormat::ISODateWithMs);
        break;
    case TimeStampInMessagesUtils::FormatType::LongDate:
        dateTimeStr = dateTime.date().toString(u"dd/MM/yyyy"_s);
        break;
    case TimeStampInMessagesUtils::FormatType::ShortDate:
        dateTimeStr = dateTime.date().toString(Qt::DateFormat::ISODate);
        break;
    case TimeStampInMessagesUtils::FormatType::LongTime:
        dateTimeStr = dateTime.time().toString(Qt::DateFormat::ISODateWithMs);
        break;
    case TimeStampInMessagesUtils::FormatType::ShortTime:
        dateTimeStr = dateTime.time().toString(u"hh:mm"_s);
        break;
    case TimeStampInMessagesUtils::FormatType::RelativeTime:
        // TODO
        dateTimeStr = dateTime.toString(Qt::DateFormat::ISODateWithMs);
        break;
    case TimeStampInMessagesUtils::FormatType::Unknown:
        return {};
    }

    return u"`%1`"_s.arg(dateTimeStr);
}
