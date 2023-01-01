/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussiontest.h"
#include "discussions/discussion.h"
#include <QTest>

QTEST_GUILESS_MAIN(DiscussionTest)
DiscussionTest::DiscussionTest(QObject *parent)
    : QObject(parent)
{
}

void DiscussionTest::shouldHaveDefaultValue()
{
    Discussion w;
    QVERIFY(w.description().isEmpty());
    QVERIFY(w.parentRoomId().isEmpty());
    QCOMPARE(w.numberMessages(), 0);
    QCOMPARE(w.lastMessage(), -1);
    QVERIFY(w.discussionRoomId().isEmpty());
    // TODO
}
