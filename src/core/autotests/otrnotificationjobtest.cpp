/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "otrnotificationjobtest.h"
#include "otr/otrnotificationjob.h"
#include <QTest>
QTEST_MAIN(OtrNotificationJobTest)
OtrNotificationJobTest::OtrNotificationJobTest(QObject *parent)
    : QObject{parent}
{
}

void OtrNotificationJobTest::shouldHaveDefaultValues()
{
    OtrNotificationJob job;
    QVERIFY(!job.canStart());
}

#include "moc_otrnotificationjobtest.cpp"
