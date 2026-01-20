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
        const TimeStampInMessagesUtils::TimeStampInfo emptyInfo{
            .format = u"f"_s,
            .date = QDate(2026, 12, 25),
            .time = QTime(1, 5, 6, 10),
            .timeZone = u"-10:00"_s,
        };
        QTest::addRow("test1") << emptyInfo << u"<t:2026-12-25T01:05:06.010-10:00:f>"_s;
    }
}
