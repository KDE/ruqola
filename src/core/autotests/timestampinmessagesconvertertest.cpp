/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesconvertertest.h"
#include "timestamp/timestampinmessagesconverter.h"
#include <QTest>
QTEST_GUILESS_MAIN(TimeStampInMessagesConverterTest)
using namespace Qt::Literals::StringLiterals;
TimeStampInMessagesConverterTest::TimeStampInMessagesConverterTest(QObject *parent)
    : QObject{parent}
{
}

void TimeStampInMessagesConverterTest::shouldConvertTimeStampInMessages_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("convertedText");
    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("test1") << u"<t:2026-01-23T13:57:56.873:t>"_s << u"`2026-01-23T13:57:56.873`"_s;
    QTest::newRow("test2") << u"<t:2026-01-23T13:57:56.873:t> foo <t:2025-08-23T13:57:56.873:R>"_s
                           << u"`2026-01-23T13:57:56.873` foo `2025-08-23T13:57:56.873`"_s;
    QTest::newRow("test3") << u"<t:2022-01-23T13:58:56.873:d>"_s << u"`2022-01-23T13:58:56.873`"_s;
    QTest::newRow("test4") << u"<t:2022-01-23T13:58:56.873:D>"_s << u"`23/01/2022 13:58`"_s;
}

void TimeStampInMessagesConverterTest::shouldConvertTimeStampInMessages()
{
    QFETCH(QString, input);
    QFETCH(QString, convertedText);
    const TimeStampInMessagesConverter converter;
    QCOMPARE(converter.generateTimeStamp(input), convertedText);
}

void TimeStampInMessagesConverterTest::shouldConvertTimeStamp_data()
{
    QTest::addColumn<QDateTime>("dateTime");
    QTest::addColumn<TimeStampInMessagesUtils::FormatType>("format");
    QTest::addColumn<QString>("result");

    QTest::newRow("empty") << QDateTime() << TimeStampInMessagesUtils::FormatType::Unknown << QString();
    QTest::newRow("empty-test1") << QDateTime(QDate(2024, 5, 5), QTime(5, 5, 5)) << TimeStampInMessagesUtils::FormatType::Unknown << QString();
    QTest::newRow("test1") << QDateTime(QDate(2024, 5, 5), QTime(5, 5, 5)) << TimeStampInMessagesUtils::FormatType::LongDate << u"`05/05/2024 05:05`"_s;
}

void TimeStampInMessagesConverterTest::shouldConvertTimeStamp()
{
    QFETCH(QDateTime, dateTime);
    QFETCH(TimeStampInMessagesUtils::FormatType, format);
    QFETCH(QString, result);

    const TimeStampInMessagesConverter converter;
    QCOMPARE(converter.convertTimeStamp(dateTime, format), result);
}

#include "moc_timestampinmessagesconvertertest.cpp"
