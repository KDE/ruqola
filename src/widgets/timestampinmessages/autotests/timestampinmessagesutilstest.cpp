/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesutilstest.h"
#include "timestampinmessages/timestampinmessagesutils.h"
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

#include "moc_timestampinmessagesutilstest.cpp"
