/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencetest.h"
#include "ruqola_autotest_helper.h"
#include "videoconference/videoconference.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
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
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/videoconference/"_L1 + name + ".json"_L1;
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
    QTest::addRow("conferenceempty") << u"conferenceempty"_s << VideoConference();
    {
        VideoConference conference;
        conference.setAction(VideoConference::Action::IncomingCall);
        conference.setCallId("6398e61e29673367a61e6f23"_ba);
        conference.setRoomId("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"_ba);
        conference.setUserId("YbwG4T2uB3wZSZSKB"_ba);
        QTest::addRow("call") << u"call"_s << conference;
    }
    {
        VideoConference conference;
        conference.setAction(VideoConference::Action::Accepted);
        conference.setCallId("6398e79daa2274c5a48a02c2"_ba);
        conference.setRoomId("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"_ba);
        conference.setUserId("YbwG4T2uB3wZSZSKB"_ba);
        QTest::addRow("accept") << u"accept"_s << conference;
    }
    {
        VideoConference conference;
        conference.setAction(VideoConference::Action::Confirmed);
        conference.setCallId("6398e79daa2274c5a48a02c2"_ba);
        conference.setRoomId("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"_ba);
        conference.setUserId("xkNpoB3T98EEPCj2K"_ba);
        QTest::addRow("confirmed") << u"confirmed"_s << conference;
    }
    {
        VideoConference conference;
        conference.setAction(VideoConference::Action::Rejected);
        conference.setCallId("6398e888fc94d2587faae3c6"_ba);
        conference.setRoomId("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"_ba);
        conference.setUserId("YbwG4T2uB3wZSZSKB"_ba);
        QTest::addRow("rejected") << u"rejected"_s << conference;
    }
    {
        VideoConference conference;
        conference.setAction(VideoConference::Action::Canceled);
        conference.setCallId("6398e944aa2274c5a48a02ea"_ba);
        conference.setRoomId("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"_ba);
        conference.setUserId("YbwG4T2uB3wZSZSKB"_ba);
        QTest::addRow("canceled") << u"canceled"_s << conference;
    }
}

#include "moc_videoconferencetest.cpp"
