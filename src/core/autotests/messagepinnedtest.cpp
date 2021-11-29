/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
