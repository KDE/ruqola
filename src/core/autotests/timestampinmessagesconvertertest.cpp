/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "timestampinmessagesconvertertest.h"
#include <QTest>
QTEST_GUILESS_MAIN(TimeStampInMessagesConverterTest)

TimeStampInMessagesConverterTest::TimeStampInMessagesConverterTest(QObject *parent)
    : QObject{parent}
{
}

#include "moc_timestampinmessagesconvertertest.cpp"
