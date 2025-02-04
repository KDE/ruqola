/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechanneldescriptionjobtest.h"
#include "channels/changechanneldescriptionjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChangeChannelDescriptionJobTest)
using namespace RocketChatRestApi;
ChangeChannelDescriptionJobTest::ChangeChannelDescriptionJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeChannelDescriptionJobTest::shouldNotStarting()
{
    ChangeChannelDescriptionJob job;

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

void ChangeChannelDescriptionJobTest::shouldHaveDefaultValue()
{
    ChangeChannelDescriptionJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.description().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeChannelDescriptionJobTest::shouldGenerateRequest()
{
    ChangeChannelDescriptionJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setDescription")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeChannelDescriptionJobTest::shouldGenerateJson()
{
    ChangeChannelDescriptionJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString description = QStringLiteral("topic1");
    job.setRoomId(roomId);
    job.setDescription(description);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"description":"%1","roomId":"%2"})").arg(description, roomId).toLatin1());
}

#include "moc_changechanneldescriptionjobtest.cpp"
