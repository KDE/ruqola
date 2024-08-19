/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserinfostest.h"
#include "moderation/moderationreportuserinfos.h"
#include <QTest>
QTEST_GUILESS_MAIN(ModerationReportUserInfosTest)
ModerationReportUserInfosTest::ModerationReportUserInfosTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationReportUserInfosTest::shouldHaveDefaultValues()
{
    ModerationReportUserInfos w;
    QCOMPARE(w.moderationReportUserInfosCount(), 0);
    QCOMPARE(w.offset(), 0);
    QCOMPARE(w.total(), 0);
    QVERIFY(w.moderationReportUserInfosList().isEmpty());
    QVERIFY(!w.user().isValid());
}

#include "moc_moderationreportuserinfostest.cpp"
