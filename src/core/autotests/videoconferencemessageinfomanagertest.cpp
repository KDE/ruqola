/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencemessageinfomanagertest.h"
#include "videoconference/videoconferencemessageinfomanager.h"
#include <QTest>
QTEST_MAIN(VideoConferenceMessageInfoManagerTest)
VideoConferenceMessageInfoManagerTest::VideoConferenceMessageInfoManagerTest(QObject *parent)
    : QObject{parent}
{
}

void VideoConferenceMessageInfoManagerTest::shouldHaveDefaultValues()
{
    VideoConferenceMessageInfoManager w(nullptr);
    QVERIFY(!w.rocketChatAccount());
}
