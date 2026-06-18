/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsunbanuserjobtest.h"

#include "restapimethod.h"
#include "rooms/roomsunbanuserjob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(RoomsUnbanUserJobTest)
using namespace RocketChatRestApi;
RoomsUnbanUserJobTest::RoomsUnbanUserJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsUnbanUserJobTest::shouldHaveDefaultValue()
{
    RoomsUnbanUserJob job;
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.userName().isEmpty());
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RoomsUnbanUserJobTest::shouldGenerateRequest()
{
    RoomsUnbanUserJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.unbanUser"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RoomsUnbanUserJobTest::shouldGenerateJson()
{
    {
        RoomsUnbanUserJob job;

        const QByteArray roomId("rr1"_ba);
        job.setRoomId(roomId);
        const QString userName(u"foo1"_s);
        job.setUserName(userName);

        QCOMPARE(job.json().toJson(QJsonDocument::Compact),
                 QStringLiteral(R"({"roomId":"%1","username":"%2"})").arg(QString::fromLatin1(roomId), userName).toLatin1());
    }
}

void RoomsUnbanUserJobTest::shouldNotStarting()
{
    RoomsUnbanUserJob job;

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

    const QByteArray roomId("room1");
    job.setRoomId(roomId);
    QVERIFY(!job.canStart());
    const QString username(u"file25"_s);
    job.setUserName(username);
    QVERIFY(job.canStart());
}

#include "moc_roomsunbanuserjobtest.cpp"
