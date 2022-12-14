/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfotest.h"
#include "videoconference/videoconferenceinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(VideoConferenceInfoTest)
VideoConferenceInfoTest::VideoConferenceInfoTest(QObject *parent)
    : QObject{parent}
{
}

void VideoConferenceInfoTest::shouldHaveDefaultValues()
{
    VideoConferenceInfo info;
    QVERIFY(info.url().isEmpty());
    QCOMPARE(info.status(), -1);
    QVERIFY(!info.ringing());
    QVERIFY(info.roomId().isEmpty());
    // TODO
}
