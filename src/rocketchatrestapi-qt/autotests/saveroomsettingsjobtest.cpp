/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "saveroomsettingsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rooms/saveroomsettingsjob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(SaveRoomSettingsJobTest)
using namespace RocketChatRestApi;
SaveRoomSettingsJobTest::SaveRoomSettingsJobTest(QObject *parent)
    : QObject(parent)
{
}

void SaveRoomSettingsJobTest::shouldHaveDefaultValue()
{
    SaveRoomSettingsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(!job.saveRoomSettingsInfo().isValid());

    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.saveRoomSettingsInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void SaveRoomSettingsJobTest::shouldGenerateRequest()
{
    SaveRoomSettingsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.saveRoomSettings"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SaveRoomSettingsJobTest::shouldGenerateJson()
{
    SaveRoomSettingsJob job;
    const QByteArray roomId("foo1");
    SaveRoomSettingsJob::SaveRoomSettingsInfo info;
    info.roomId = roomId;
    job.setSaveRoomSettingsInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"favorite":{},"rid":"%1","roomType":""})").arg(QLatin1StringView(roomId)).toLatin1());

    const QString roomAnnouncement = u"announcement"_s;
    info.mSettingsWillBeChanged |= SaveRoomSettingsJob::SaveRoomSettingsInfo::RoomAnnouncement;
    info.roomAnnouncement = roomAnnouncement;
    job.setSaveRoomSettingsInfo(info);
    const QStringList systemMessages{u"ua"_s, u"ca"_s};
    info.mSettingsWillBeChanged |= SaveRoomSettingsJob::SaveRoomSettingsInfo::SystemMessages;
    info.systemMessages = systemMessages;
    job.setSaveRoomSettingsInfo(info);
    QString res;
    for (const QString &s : systemMessages) {
        if (!res.isEmpty()) {
            res += u',';
        }
        res += u"\"%1\""_s.arg(s);
    }
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"favorite":{},"rid":"%1","roomAnnouncement":"%2","roomType":"","systemMessages":[%3]})")
                 .arg(QLatin1StringView(roomId), roomAnnouncement, res)
                 .toLatin1());
}

void SaveRoomSettingsJobTest::shouldNotStarting()
{
    SaveRoomSettingsJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QByteArray roomId("foo1");
    SaveRoomSettingsJob::SaveRoomSettingsInfo info;
    info.roomId = roomId;
    job.setSaveRoomSettingsInfo(info);
    QVERIFY(!job.canStart());

    // We need to change a settings
    //     QVERIFY(job.canStart());
}

#include "moc_saveroomsettingsjobtest.cpp"
