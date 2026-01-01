/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "repliestest.h"
#include "messages/replies.h"
#include <QTest>
QTEST_GUILESS_MAIN(RepliesTest)

RepliesTest::RepliesTest(QObject *parent)
    : QObject{parent}
{
}

void RepliesTest::shouldHaveDefaultValues()
{
    Replies w;
    QVERIFY(w.replies().isEmpty());
    QVERIFY(w.isEmpty());
}

#include "moc_repliestest.cpp"
