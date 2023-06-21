/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagepinnedtest.h"
#include "messages/messagepinned.h"
#include <QTest>
QTEST_GUILESS_MAIN(MessagePinnedTest)

MessagePinnedTest::MessagePinnedTest(QObject *parent)
    : QObject(parent)
{
}

void MessagePinnedTest::shouldHaveDefaultValue()
{
    MessagePinned p;
    QVERIFY(!p.pinned());
    QVERIFY(p.pinnedBy().isEmpty());
}

#include "moc_messagepinnedtest.cpp"
