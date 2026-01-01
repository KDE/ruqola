/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageurlstest.h"
#include "messages/messageurls.h"
#include <QTest>
QTEST_GUILESS_MAIN(MessageUrlsTest)

MessageUrlsTest::MessageUrlsTest(QObject *parent)
    : QObject{parent}
{
}

void MessageUrlsTest::shouldHaveDefaultValue()
{
    MessageUrls w;
    QVERIFY(w.isEmpty());
    QVERIFY(w.messageUrls().isEmpty());
}

#include "moc_messageurlstest.cpp"
