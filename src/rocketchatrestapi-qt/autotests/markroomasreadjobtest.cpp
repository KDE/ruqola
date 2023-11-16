/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "markroomasreadjobtest.h"
#include "ruqola_restapi_helper.h"
#include "subscriptions/markroomasreadjob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(MarkRoomAsReadJobTest)
using namespace RocketChatRestApi;
MarkRoomAsReadJobTest::MarkRoomAsReadJobTest(QObject *parent)
    : QObject(parent)
{
}

void MarkRoomAsReadJobTest::shouldHaveDefaultValue()
{
    MarkRoomAsReadJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void MarkRoomAsReadJobTest::shouldGenerateRequest()
{
    MarkRoomAsReadJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/subscriptions.read")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void MarkRoomAsReadJobTest::shouldGenerateJson()
{
    MarkRoomAsReadJob job;
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"rid":"%1"})").arg(roomId).toLatin1());
}

void MarkRoomAsReadJobTest::shouldNotStarting()
{
    MarkRoomAsReadJob job;

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

#include "moc_markroomasreadjobtest.cpp"
