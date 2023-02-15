/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfotest.h"
#include "ruqola_autotest_helper.h"
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
    QCOMPARE(info.conferenceType(), VideoConferenceInfo::VideoConferenceType::Unknown);
    QVERIFY(info.providerName().isEmpty());
    QVERIFY(info.users().isEmpty());
    QVERIFY(info.messageId().isEmpty());
    QVERIFY(info.blockId().isEmpty());
    QVERIFY(!info.isValid());
}

void VideoConferenceInfoTest::shouldLoadVideoConferenceInfo_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<VideoConferenceInfo>("videoconferenceInfo");
    {
        VideoConferenceInfo info;
        info.setProviderName(QStringLiteral("jitsi"));
        info.setUrl(QStringLiteral("https://bla/RocketChat63ebad0405c6c93252c0f337"));
        info.setConferenceType(VideoConferenceInfo::Conference);
        info.setRoomId(QStringLiteral("hE6RS3iv5ND5EGWC6"));
        info.setStatus(1);
        info.setRinging(false);
        info.setCreatedAtDateTime(1676389636880);
        info.setMessageId(QStringLiteral("NusivujrtX38F8too"));
        info.setBlockId(QStringLiteral("63ebad0405c6c93252c0f337"));
        QTest::addRow("videoconferenceinfo") << QStringLiteral("videoconferenceinfo") << info;
    }
    {
        VideoConferenceInfo info;
        info.setProviderName(QStringLiteral("jitsi"));
        info.setUrl(QStringLiteral("https://goo/RocketChat639593184ef3f3baa965910c"));
        info.setConferenceType(VideoConferenceInfo::Conference);
        info.setRoomId(QStringLiteral("hE6RS3iv5ND5EGWC6"));
        info.setStatus(2);
        info.setRinging(false);
        info.setCreatedAtDateTime(1670746904785);
        info.setEndedAtDateTime(1670835600213);
        User user;
        user.setUserName(QStringLiteral("laurent"));
        user.setName(QStringLiteral("Laurent Montel"));
        user.setUserId(QStringLiteral("uKK39zoewTkdacidH"));
        user.setUtcOffset(0);
        user.setActive(true);
        user.setStatus(User::PresenceStatus::Unknown);
        info.setUsers({user});
        info.setMessageId(QStringLiteral("PmWesYrnRJkyTTi"));
        info.setBlockId(QStringLiteral("639593184ef3f3baa965910c"));
        QTest::addRow("videoconferenceinfo1") << QStringLiteral("videoconferenceinfo1") << info;
    }
}

void VideoConferenceInfoTest::shouldLoadVideoConferenceInfo()
{
    QFETCH(QString, fileName);
    QFETCH(VideoConferenceInfo, videoconferenceInfo);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/videoconferenceinfo/") + fileName + QLatin1String(".json");
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
