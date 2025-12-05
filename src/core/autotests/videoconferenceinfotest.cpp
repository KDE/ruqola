/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfotest.h"
#include "ruqola_autotest_helper.h"
#include "videoconference/videoconferenceinfo.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
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
    QCOMPARE(info.conferenceType(), VideoConferenceInfo::VideoConferenceType::Unknown);
    QVERIFY(info.providerName().isEmpty());
    QVERIFY(info.users().isEmpty());
    QVERIFY(info.messageId().isEmpty());
    QVERIFY(info.blockId().isEmpty());
    QVERIFY(!info.isValid());
    QVERIFY(!info.canJoin());
}

void VideoConferenceInfoTest::shouldLoadVideoConferenceInfo_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<VideoConferenceInfo>("videoconferenceInfo");
    {
        VideoConferenceInfo info;
        info.setProviderName(u"jitsi"_s);
        info.setUrl(u"https://bla/RocketChat63ebad0405c6c93252c0f337"_s);
        info.setConferenceType(VideoConferenceInfo::VideoConferenceType::Conference);
        info.setRoomId(u"hE6RS3iv5ND5EGWC6"_s);
        info.setStatus(1);
        info.setRinging(false);
        info.setCreatedAtDateTime(1676389636880);
        info.setMessageId(u"NusivujrtX38F8too"_s);
        info.setBlockId(u"63ebad0405c6c93252c0f337"_s);
        QTest::addRow("videoconferenceinfo") << u"videoconferenceinfo"_s << info;
    }
    {
        VideoConferenceInfo info;
        info.setProviderName(u"jitsi"_s);
        info.setUrl(u"https://goo/RocketChat639593184ef3f3baa965910c"_s);
        info.setConferenceType(VideoConferenceInfo::VideoConferenceType::Conference);
        info.setRoomId(u"hE6RS3iv5ND5EGWC6"_s);
        info.setStatus(2);
        info.setRinging(false);
        info.setCreatedAtDateTime(1670746904785);
        info.setEndedAtDateTime(1670835600213);
        User user;
        user.setUserName(u"laurent"_s);
        user.setName(u"Laurent Montel"_s);
        user.setUserId("uKK39zoewTkdacidH"_ba);
        user.setUtcOffset(0);
        user.setActive(true);
        user.setStatus(User::PresenceStatus::Unknown);
        info.setUsers({user});
        info.setMessageId(u"PmWesYrnRJkyTTi"_s);
        info.setBlockId(u"639593184ef3f3baa965910c"_s);
        QTest::addRow("videoconferenceinfo1") << u"videoconferenceinfo1"_s << info;
    }
}

void VideoConferenceInfoTest::shouldLoadVideoConferenceInfo()
{
    QFETCH(QString, fileName);
    QFETCH(VideoConferenceInfo, videoconferenceInfo);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/videoconferenceinfo/"_L1 + fileName + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    VideoConferenceInfo r;
    r.parse(obj);
    const bool result = (r == videoconferenceInfo);
    if (!result) {
        qDebug() << "result " << r;
        qDebug() << "expected " << videoconferenceInfo;
    }
    QVERIFY(result);
}

#include "moc_videoconferenceinfotest.cpp"
