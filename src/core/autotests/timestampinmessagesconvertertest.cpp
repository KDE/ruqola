/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesconvertertest.h"
#include "timestamp/timestampinmessagesconverter.h"
#include <QTest>
QTEST_GUILESS_MAIN(TimeStampInMessagesConverterTest)

TimeStampInMessagesConverterTest::TimeStampInMessagesConverterTest(QObject *parent)
    : QObject{parent}
{
}

void TimeStampInMessagesConverterTest::shouldConvertTimeStampInMessages_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("convertedText");
    QTest::newRow("empty") << QString() << QString();
}

void TimeStampInMessagesConverterTest::shouldConvertTimeStampInMessages()
{
    QFETCH(QString, input);
    QFETCH(QString, convertedText);
    TimeStampInMessagesConverter converter;
    QCOMPARE(converter.generateTimeStamp(input), convertedText);
}

#include "moc_timestampinmessagesconvertertest.cpp"
