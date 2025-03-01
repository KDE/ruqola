/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "postmessagejobtest.h"
#include "chat/postmessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(PostMessageJobTest)
using namespace RocketChatRestApi;
PostMessageJobTest::PostMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void PostMessageJobTest::shouldHaveDefaultValue()
{
    PostMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomIds().isEmpty());
    QVERIFY(job.text().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void PostMessageJobTest::shouldGenerateRequest()
{
    PostMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.postMessage")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void PostMessageJobTest::shouldGenerateJson()
{
    PostMessageJob job;
    const QByteArray roomId("foo1");
    const QString text = QStringLiteral("topic1");
    job.setRoomIds({roomId});
    job.setText(text);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","text":"%2"})").arg(QLatin1StringView(roomId), text).toLatin1());
}

void PostMessageJobTest::shouldNotStarting()
{
    PostMessageJob job;

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
    const QByteArray roomId("foo1");
    job.setRoomIds({roomId});
    QVERIFY(!job.canStart());
    const QString text = QStringLiteral("topic1");
    job.setText(text);
    QVERIFY(job.canStart());
}

#include "moc_postmessagejobtest.cpp"
