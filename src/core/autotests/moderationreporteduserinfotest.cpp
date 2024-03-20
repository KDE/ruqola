/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

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
    QCOMPARE(w.timeStamp(), -1);
    QCOMPARE(w.count(), 0);
    // TODO
}
