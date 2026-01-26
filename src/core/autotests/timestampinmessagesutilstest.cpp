/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesutilstest.h"
#include "timestamp/timestampinmessagesutils.h"
#include <QTest>
QTEST_GUILESS_MAIN(TimeStampInMessagesUtilsTest)
using namespace Qt::Literals::StringLiterals;
TimeStampInMessagesUtilsTest::TimeStampInMessagesUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void TimeStampInMessagesUtilsTest::shouldGenerateTimeStampStr()
{
    QFETCH(TimeStampInMessagesUtils::TimeStampInfo, info);
    QFETCH(QString, result);

    QCOMPARE(TimeStampInMessagesUtils::generateTimeStampStr(info), result);
}

void TimeStampInMessagesUtilsTest::shouldGenerateTimeStampStr_data()
{
    QTest::addColumn<TimeStampInMessagesUtils::TimeStampInfo>("info");
    QTest::addColumn<QString>("result");

    {
        const TimeStampInMessagesUtils::TimeStampInfo emptyInfo;
        QTest::addRow("empty") << emptyInfo << QString();
    }
    {
        const TimeStampInMessagesUtils::TimeStampInfo info{
            .format = TimeStampInMessagesUtils::FormatType::FullDateTime,
            .date = QDate(2026, 12, 25),
            .time = QTime(1, 5, 6, 10),
            .timeZone = u"-10:00"_s,
        };
        QTest::addRow("test1") << info << u"<t:2026-12-25T01:05:06.010-10:00:f>"_s;
    }
    {
        const TimeStampInMessagesUtils::TimeStampInfo info{
            .format = TimeStampInMessagesUtils::FormatType::LongDate,
            .date = QDate(2026, 12, 25),
            .time = QTime(1, 5, 6, 10),
            .timeZone = u"-10:00"_s,
        };
        QTest::addRow("test2") << info << u"<t:2026-12-25T01:05:06.010-10:00:D>"_s;
    }
    {
        const TimeStampInMessagesUtils::TimeStampInfo info{
            .format = TimeStampInMessagesUtils::FormatType::RelativeTime,
            .date = QDate(2026, 11, 25),
            .time = QTime(1, 6, 10, 10),
            .timeZone = u"-10:00"_s,
        };
        QTest::addRow("test3") << info << u"<t:2026-11-25T01:06:10.010-10:00:R>"_s;
    }
}

void TimeStampInMessagesUtilsTest::shouldConvertFormatTypeToString()
{
    QCOMPARE(TimeStampInMessagesUtils::convertFormatTypeToString(TimeStampInMessagesUtils::FormatType::FullDateTime), u'f');
    QCOMPARE(TimeStampInMessagesUtils::convertFormatTypeToString(TimeStampInMessagesUtils::FormatType::LongFullDateTime), u'F');
    QCOMPARE(TimeStampInMessagesUtils::convertFormatTypeToString(TimeStampInMessagesUtils::FormatType::LongDate), u'D');
    QCOMPARE(TimeStampInMessagesUtils::convertFormatTypeToString(TimeStampInMessagesUtils::FormatType::LongTime), u'T');
    QCOMPARE(TimeStampInMessagesUtils::convertFormatTypeToString(TimeStampInMessagesUtils::FormatType::ShortDate), u'd');
    QCOMPARE(TimeStampInMessagesUtils::convertFormatTypeToString(TimeStampInMessagesUtils::FormatType::LongDate), u'D');
    QCOMPARE(TimeStampInMessagesUtils::convertFormatTypeToString(TimeStampInMessagesUtils::FormatType::RelativeTime), u'R');
    QCOMPARE(TimeStampInMessagesUtils::convertFormatTypeToString(TimeStampInMessagesUtils::FormatType::Unknown), QString());
}

void TimeStampInMessagesUtilsTest::shouldConvertStringToFormatType()
{
    QCOMPARE(TimeStampInMessagesUtils::convertStringToFormatType(u"f"_s), TimeStampInMessagesUtils::FormatType::FullDateTime);
    QCOMPARE(TimeStampInMessagesUtils::convertStringToFormatType(u"F"_s), TimeStampInMessagesUtils::FormatType::LongFullDateTime);
    QCOMPARE(TimeStampInMessagesUtils::convertStringToFormatType(u"d"_s), TimeStampInMessagesUtils::FormatType::ShortDate);
    QCOMPARE(TimeStampInMessagesUtils::convertStringToFormatType(u"D"_s), TimeStampInMessagesUtils::FormatType::LongDate);
    QCOMPARE(TimeStampInMessagesUtils::convertStringToFormatType(u"t"_s), TimeStampInMessagesUtils::FormatType::ShortTime);
    QCOMPARE(TimeStampInMessagesUtils::convertStringToFormatType(u"T"_s), TimeStampInMessagesUtils::FormatType::LongTime);
    QCOMPARE(TimeStampInMessagesUtils::convertStringToFormatType(u"R"_s), TimeStampInMessagesUtils::FormatType::RelativeTime);
    QCOMPARE(TimeStampInMessagesUtils::convertStringToFormatType(QString()), TimeStampInMessagesUtils::FormatType::Unknown);
    QCOMPARE(TimeStampInMessagesUtils::convertStringToFormatType(u"foo"_s), TimeStampInMessagesUtils::FormatType::Unknown);
}

#include "moc_timestampinmessagesutilstest.cpp"
