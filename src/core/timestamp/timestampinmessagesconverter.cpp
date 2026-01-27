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

QString TimeStampInMessagesConverter::generateTimeStamp(const QString &str) const
{
    if (str.isEmpty()) {
        return {};
    }

    const static QRegularExpression reg(u"<t:([^>]*?)(?::([tTdDFfR]))?>"_s);

    QRegularExpressionMatchIterator roomIterator = reg.globalMatch(str);
    while (roomIterator.hasNext()) {
        const QRegularExpressionMatch match = roomIterator.next();
        const QStringView format = match.capturedView(1);
        const QStringView word = match.capturedView(2);
        qDebug() << "word " << word << " format " << format;
    }

    // TODO
    // TODO extract date info
    // TODO use regularexpression <t:<date>:<format>>
    return {};
}

QString TimeStampInMessagesConverter::convertTimeStamp(const QString &dateTime, TimeStampInMessagesUtils::FormatType type) const
{
    QString dateTimeStr;
    switch (type) {
    case TimeStampInMessagesUtils::FormatType::FullDateTime:
        dateTimeStr = QDateTime::fromString(dateTime).toString(Qt::DateFormat::ISODate);
        break;
    case TimeStampInMessagesUtils::FormatType::LongFullDateTime:
        dateTimeStr = QDateTime::fromString(dateTime).toString(Qt::DateFormat::ISODateWithMs);
        break;
    case TimeStampInMessagesUtils::FormatType::LongDate:
        // TODO
        dateTimeStr = QDateTime::fromString(dateTime).toString(Qt::DateFormat::ISODate);
        break;
    case TimeStampInMessagesUtils::FormatType::ShortDate:
        // TODO
        dateTimeStr = QDateTime::fromString(dateTime).toString(Qt::DateFormat::ISODateWithMs);
        break;
    case TimeStampInMessagesUtils::FormatType::LongTime:
        // TODO
        dateTimeStr = QDateTime::fromString(dateTime).toString(Qt::DateFormat::ISODateWithMs);
        break;
    case TimeStampInMessagesUtils::FormatType::ShortTime:
        // TODO
        dateTimeStr = QDateTime::fromString(dateTime).toString(Qt::DateFormat::ISODateWithMs);
        break;
    case TimeStampInMessagesUtils::FormatType::RelativeTime:
        // TODO
        dateTimeStr = QDateTime::fromString(dateTime).toString(Qt::DateFormat::ISODateWithMs);
        break;
    case TimeStampInMessagesUtils::FormatType::Unknown:
        break;
    }

    return u"`%1`"_s.arg(dateTimeStr);
}
