/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "moderationreporteduserinfotest.h"
#include "moderation/moderationreporteduserinfo.h"
#include <QTest>

QTEST_GUILESS_MAIN(ModerationReportedUserInfoTest)

ModerationReportedUserInfoTest::ModerationReportedUserInfoTest(QObject *parent)
    : QObject{parent}
{
}

void ModerationReportedUserInfoTest::shouldHaveDefaultValues()
{
    ModerationReportedUserInfo w;
    QCOMPARE(w.createdAt(), -1);
    QCOMPARE(w.count(), 0);
    QVERIFY(w.createAtDisplayDateTime().isEmpty());
    QVERIFY(w.username().isEmpty());
    QVERIFY(w.name().isEmpty());
    QVERIFY(w.identifier().isEmpty());
}

#include "moc_moderationreporteduserinfotest.cpp"
