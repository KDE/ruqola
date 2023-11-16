/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencetest.h"
#include "ruqola_autotest_helper.h"
#include "videoconference/videoconference.h"

QTEST_GUILESS_MAIN(VideoConferenceTest)
VideoConferenceTest::VideoConferenceTest(QObject *parent)
    : QObject{parent}
{
}

void VideoConferenceTest::shouldHaveDefaultValues()
{
    VideoConference w;
    QVERIFY(w.callId().isEmpty());
    QVERIFY(w.roomId().isEmpty());
    QVERIFY(w.userId().isEmpty());
    QVERIFY(!w.isValid());
}

void VideoConferenceTest::shouldExtractVideoConference()
{
    QFETCH(QString, name);
    QFETCH(VideoConference, videoConference);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/videoconference/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    VideoConference result;
    result.parseVideoConference(obj);
    const bool equal = result == videoConference;
    if (!equal) {
        qDebug() << " result " << result;
        qDebug() << " deviceInfo " << videoConference;
    }
    QVERIFY(equal);
}

void VideoConferenceTest::shouldExtractVideoConference_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<VideoConference>("videoConference");
    QTest::addRow("conferenceempty") << QStringLiteral("conferenceempty") << VideoConference();
    {
        VideoConference conference;
        conference.setAction(VideoConference::IncomingCall);
        conference.setCallId(QStringLiteral("6398e61e29673367a61e6f23"));
        conference.setRoomId(QStringLiteral("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"));
        conference.setUserId(QStringLiteral("YbwG4T2uB3wZSZSKB"));
        QTest::addRow("call") << QStringLiteral("call") << conference;
    }
    {
        VideoConference conference;
        conference.setAction(VideoConference::Accepted);
        conference.setCallId(QStringLiteral("6398e79daa2274c5a48a02c2"));
        conference.setRoomId(QStringLiteral("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"));
        conference.setUserId(QStringLiteral("YbwG4T2uB3wZSZSKB"));
        QTest::addRow("accept") << QStringLiteral("accept") << conference;
    }
    {
        VideoConference conference;
        conference.setAction(VideoConference::Confirmed);
        conference.setCallId(QStringLiteral("6398e79daa2274c5a48a02c2"));
        conference.setRoomId(QStringLiteral("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"));
        conference.setUserId(QStringLiteral("xkNpoB3T98EEPCj2K"));
        QTest::addRow("confirmed") << QStringLiteral("confirmed") << conference;
    }
    {
        VideoConference conference;
        conference.setAction(VideoConference::Rejected);
        conference.setCallId(QStringLiteral("6398e888fc94d2587faae3c6"));
        conference.setRoomId(QStringLiteral("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"));
        conference.setUserId(QStringLiteral("YbwG4T2uB3wZSZSKB"));
        QTest::addRow("rejected") << QStringLiteral("rejected") << conference;
    }
    {
        VideoConference conference;
        conference.setAction(VideoConference::Canceled);
        conference.setCallId(QStringLiteral("6398e944aa2274c5a48a02ea"));
        conference.setRoomId(QStringLiteral("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"));
        conference.setUserId(QStringLiteral("YbwG4T2uB3wZSZSKB"));
        QTest::addRow("canceled") << QStringLiteral("canceled") << conference;
    }
}

#include "moc_videoconferencetest.cpp"
