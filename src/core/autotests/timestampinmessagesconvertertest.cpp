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
    QTest::newRow("test1") << u"<t:2026-01-23T13:57:56.873:t>"_s << u"`13:57`"_s;
    // Relative we will not be able to test it: QTest::newRow("test2") << u"<t:2026-01-23T13:57:56.873:t> foo <t:2025-08-23T13:57:56.873:R>"_s << u"`13:57` foo
    // `2025-08-23T13:57:56.873`"_s;
    QTest::newRow("test3") << u"<t:2022-01-23T13:58:56.873:d>"_s << u"`2022-01-23`"_s;
    QTest::newRow("test4") << u"<t:2022-01-23T13:58:56.873:D>"_s << u"`23/01/2022 13:58`"_s;
#if 0 // It fails on CI
    QTest::newRow("test5") << u"<t:2026-02-02T11:54:57.322-12:00:t>"_s << u"`00:54`"_s;
    QTest::newRow("test6") << u"<t:2026-02-02T11:54:57.322-12:00:d>"_s << u"`2026-02-03`"_s;
#endif
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

void TimeStampInMessagesConverterTest::shouldRegularExpressionStrValue()
{
    const TimeStampInMessagesConverter converter;
    QCOMPARE(converter.regularExpressionStr(), u"<t:([^>]*?)(?::([tTdDFfR]))?>"_s);
}

void TimeStampInMessagesConverterTest::shouldCalculateRelativeTime_data()
{
    QTest::addColumn<QDateTime>("dateTime");
    QTest::addColumn<QDateTime>("currentDateTime");
    QTest::addColumn<QString>("result");
    {
        QDateTime dt = QDateTime::currentDateTime();
        QTest::addRow("now") << dt << dt << u"Now"_s;
    }
    {
        const QDateTime currentDateTime = QDateTime(QDate(2025, 12, 12), QTime(3, 3, 3));
        const QDateTime testDateTime = QDateTime(QDate(2025, 12, 12), QTime(3, 3, 0));

        QTest::addRow("3 seconds") << testDateTime << currentDateTime << u"3 seconds"_s;
    }
    {
        const QDateTime currentDateTime = QDateTime(QDate(2025, 12, 12), QTime(3, 3, 3));
        const QDateTime testDateTime = QDateTime(QDate(2025, 12, 12), QTime(3, 2, 4));

        QTest::addRow("59 seconds") << testDateTime << currentDateTime << u"59 seconds"_s;
    }
    {
        const QDateTime currentDateTime = QDateTime(QDate(2025, 12, 12), QTime(3, 3, 3));
        const QDateTime testDateTime = QDateTime(QDate(2025, 12, 12), QTime(3, 0, 3));

        QTest::addRow("3 minutes") << testDateTime << currentDateTime << u"3 minutes"_s;
    }
    {
        const QDateTime currentDateTime = QDateTime(QDate(2025, 12, 12), QTime(3, 3, 3));
        const QDateTime testDateTime = QDateTime(QDate(2025, 12, 12), QTime(2, 3, 3));

        QTest::addRow("1 hour") << testDateTime << currentDateTime << u"1 hour"_s;
    }
    {
        const QDateTime currentDateTime = QDateTime(QDate(2025, 12, 12), QTime(3, 3, 3));
        const QDateTime testDateTime = QDateTime(QDate(2025, 12, 11), QTime(11, 3, 3));

        QTest::addRow("16 hours") << testDateTime << currentDateTime << u"16 hours"_s;
    }
    {
        const QDateTime currentDateTime = QDateTime(QDate(2025, 12, 12), QTime(3, 3, 3));
        const QDateTime testDateTime = QDateTime(QDate(2025, 12, 6), QTime(11, 3, 3));

        QTest::addRow("6 days") << testDateTime << currentDateTime << u"6 days"_s;
    }
    {
        const QDateTime currentDateTime = QDateTime(QDate(2025, 12, 12), QTime(3, 3, 3));
        const QDateTime testDateTime = QDateTime(QDate(2025, 11, 6), QTime(11, 3, 3));

        QTest::addRow("1 month") << testDateTime << currentDateTime << u"1 month"_s;
    }
    {
        const QDateTime currentDateTime = QDateTime(QDate(2026, 2, 12), QTime(3, 3, 3));
        const QDateTime testDateTime = QDateTime(QDate(2025, 11, 6), QTime(11, 3, 3));

        QTest::addRow("3 months") << testDateTime << currentDateTime << u"3 months"_s;
    }

    {
        const QDateTime currentDateTime = QDateTime(QDate(2026, 12, 12), QTime(3, 3, 3));
        const QDateTime testDateTime = QDateTime(QDate(2025, 11, 6), QTime(11, 3, 3));

        QTest::addRow("1 year") << testDateTime << currentDateTime << u"1 year"_s;
    }

    {
        const QDateTime currentDateTime = QDateTime(QDate(2027, 1, 12), QTime(3, 3, 3));
        const QDateTime testDateTime = QDateTime(QDate(2025, 11, 6), QTime(11, 3, 3));

        QTest::addRow("2 years") << testDateTime << currentDateTime << u"2 years"_s;
    }
    {
        const QDateTime currentDateTime = QDateTime(QDate(2035, 1, 12), QTime(3, 3, 3));
        const QDateTime testDateTime = QDateTime(QDate(2025, 11, 6), QTime(11, 3, 3));

        QTest::addRow("10 years") << testDateTime << currentDateTime << u"10 years"_s;
    }
}

void TimeStampInMessagesConverterTest::shouldCalculateRelativeTime()
{
    QFETCH(QDateTime, dateTime);
    QFETCH(QDateTime, currentDateTime);
    QFETCH(QString, result);
    QCOMPARE(TimeStampInMessagesConverter::calculateRelativeTime(dateTime, currentDateTime), result);
}

#include "moc_timestampinmessagesconvertertest.cpp"
