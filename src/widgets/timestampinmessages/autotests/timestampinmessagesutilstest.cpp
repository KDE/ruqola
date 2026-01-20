/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesutilstest.h"
#include "timestampinmessages/timestampinmessagesutils.h"
#include <QTest>
QTEST_GUILESS_MAIN(TimeStampInMessagesUtilsTest)

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

    TimeStampInMessagesUtils::TimeStampInfo emptyInfo;

    QTest::addRow("empty") << emptyInfo << QString();

    // TODO
}
