/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "unbanuserinchanneljobtest.h"
#include "job/unbanuserinchanneljob.h"
#include <QTest>
QTEST_GUILESS_MAIN(UnbanUserInChannelJobTest)

UnbanUserInChannelJobTest::UnbanUserInChannelJobTest(QObject *parent)
    : QObject{parent}
{
}

void UnbanUserInChannelJobTest::shouldHaveDefaultValues()
{
    const UnbanUserInChannelJob job(nullptr);
    QVERIFY(job.needUnbanUsers().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.canStart());
}

#include "moc_unbanuserinchanneljobtest.cpp"
