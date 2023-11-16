/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamupdateroomjobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamupdateroomjob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(TeamUpdateRoomJobTest)
using namespace RocketChatRestApi;
TeamUpdateRoomJobTest::TeamUpdateRoomJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamUpdateRoomJobTest::shouldHaveDefaultValue()
{
    TeamUpdateRoomJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.isDefault());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void TeamUpdateRoomJobTest::shouldGenerateRequest()
{
    TeamUpdateRoomJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.updateRoom")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void TeamUpdateRoomJobTest::shouldGenerateJson()
{
    TeamUpdateRoomJob job;
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    const bool isDefault = true;
    job.setIsDefault(isDefault);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"isDefault":true,"roomId":"%1"})").arg(roomId).toLatin1());
}

void TeamUpdateRoomJobTest::shouldNotStarting()
{
    TeamUpdateRoomJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_teamupdateroomjobtest.cpp"
