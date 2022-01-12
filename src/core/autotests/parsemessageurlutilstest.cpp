/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parsemessageurlutilstest.h"
#include "parsemessageurlutils.h"
#include <QTest>
QTEST_MAIN(ParseMessageUrlUtilsTest)
ParseMessageUrlUtilsTest::ParseMessageUrlUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void ParseMessageUrlUtilsTest::shouldHaveDefaultValues()
{
    ParseMessageUrlUtils w;
    QVERIFY(w.messageId().isEmpty());
    QVERIFY(w.roomId().isEmpty());
    QVERIFY(w.serverPath().isEmpty());
    // TODO
}
