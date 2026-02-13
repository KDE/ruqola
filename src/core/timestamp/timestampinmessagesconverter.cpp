/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "timestampinmessagesconverter.h"
#include <KLocalizedString>
#include <QDateTime>
#include <QRegularExpression>
#include <chrono>
using namespace std::chrono_literals;
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
        const QStringView dateTimeCaptured = match.capturedView(1);
        const QStringView format = match.capturedView(2);

        QDateTime dateTime = QDateTime::fromString(dateTimeCaptured.toString(), u"yyyy-MM-ddThh:mm:ss.zzz"_s);

        if (!dateTime.isValid()) {
            dateTime = QDateTime::fromString(dateTimeCaptured.toString(), u"yyyy-MM-ddThh:mm:ss.zzzttt"_s);
            // qDebug() << " dateTime " << dateTime;
        }
        const QString result = convertTimeStamp(dateTime, TimeStampInMessagesUtils::convertStringToFormatType(format.toString()));
        // qDebug() << "dateTime " << dateTime << " format " << format << " result " << result;
        // qDebug() << "match.captured(0) " << match.captured(0) << " QDateTime::fromString(dateTime.toString()) " <<
        // QDateTime::fromString(dateTime.toString()); qDebug() << " QDateTime::fromString(dateTime.toString(), udd_s) " <<
        // QDateTime::fromString(dateTime.toString(), u"yyyy-MM-ddThh:mm:ss.zzz"_s);
        newStr.replace(match.captured(0), result);
    }
    return newStr;
}

QString TimeStampInMessagesConverter::calculateRelativeTime(const QDateTime &dateTime, const QDateTime &currentDateTime)
{
    if (dateTime == currentDateTime) {
        return i18n("Now");
    }
    const auto diff = (currentDateTime - dateTime);
    if (diff < 60s) {
        return i18np("%1 second", "%1 seconds", static_cast<int>(diff.count() / 1000));
    }
    constexpr auto minutes = 60 * 60s;
    if (diff < minutes) {
        return i18np("%1 minute", "%1 minutes", static_cast<int>(diff / 60s));
    }
    constexpr auto hours = 24 * 60 * 60s;
    if (diff < hours) {
        return i18np("%1 hour", "%1 hours", static_cast<int>(diff / (60 * 60s)));
    }
    constexpr auto days = 30 * 24 * 60 * 60s;
    if (diff < days) {
        return i18np("%1 day", "%1 days", dateTime.date().daysTo(currentDateTime.date()));
    }
    if ((currentDateTime.date().year() - dateTime.date().year()) <= 1) {
        const auto currentMonth = currentDateTime.date().month();
        const auto dateMonth = dateTime.date().month();
        auto diff = 0;
        if (currentMonth > dateMonth) {
            if ((currentDateTime.date().year() - dateTime.date().year()) == 1) {
                return i18np("%1 year", "%1 years", 1);
            } else {
                diff = currentMonth - dateMonth;
            }
        } else {
            diff = 12 - (dateMonth - currentMonth);
        }
        return i18np("%1 month", "%1 months", diff);
    } else {
        return i18np("%1 year", "%1 years", currentDateTime.date().year() - dateTime.date().year());
    }

    return {};
}

QString TimeStampInMessagesConverter::convertTimeStamp(const QDateTime &dateTime, TimeStampInMessagesUtils::FormatType type)
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
        dateTimeStr = dateTime.toString(u"dd/MM/yyyy hh:mm"_s);
        break;
    case TimeStampInMessagesUtils::FormatType::ShortDate:
        dateTimeStr = dateTime.date().toString(Qt::DateFormat::ISODate);
        break;
    case TimeStampInMessagesUtils::FormatType::LongTime:
        dateTimeStr = dateTime.time().toString(Qt::DateFormat::ISODate);
        break;
    case TimeStampInMessagesUtils::FormatType::ShortTime:
        dateTimeStr = dateTime.time().toString(u"hh:mm"_s);
        break;
    case TimeStampInMessagesUtils::FormatType::RelativeTime:
        dateTimeStr = calculateRelativeTime(dateTime, QDateTime::currentDateTime());
        break;
    case TimeStampInMessagesUtils::FormatType::Unknown:
        return {};
    }

    return u"`%1`"_s.arg(dateTimeStr);
}
