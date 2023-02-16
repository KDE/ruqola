/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "updatevideoconferencemessagejobtest.h"
#include "videoconference/updatevideoconferencemessagejob.h"
#include <QTest>

QTEST_MAIN(UpdateVideoConferenceMessageJobTest)

UpdateVideoConferenceMessageJobTest::UpdateVideoConferenceMessageJobTest(QObject *parent)
    : QObject{parent}
{
}

void UpdateVideoConferenceMessageJobTest::shouldHaveDefaultValues()
{
    UpdateVideoConferenceMessageJob job;
    QVERIFY(!job.rocketChatAccount());
    QVERIFY(!job.canStart());
    QVERIFY(!job.videoConferenceInfo().isValid());
}
