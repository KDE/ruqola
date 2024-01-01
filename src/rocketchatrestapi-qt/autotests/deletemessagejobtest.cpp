/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deletemessagejobtest.h"
#include "chat/deletemessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(DeleteMessageJobTest)
using namespace RocketChatRestApi;
DeleteMessageJobTest::DeleteMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void DeleteMessageJobTest::shouldHaveDefaultValue()
{
    DeleteMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void DeleteMessageJobTest::shouldGenerateRequest()
{
    DeleteMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.delete")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void DeleteMessageJobTest::shouldGenerateJson()
{
    DeleteMessageJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString text = QStringLiteral("topic1");
    job.setRoomId(roomId);
    job.setMessageId(text);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"msgId":"%2","roomId":"%1"})").arg(roomId, text).toLatin1());
}

void DeleteMessageJobTest::shouldNotStarting()
{
    DeleteMessageJob job;

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
    QVERIFY(!job.canStart());
    const QString text = QStringLiteral("topic1");
    job.setMessageId(text);
    QVERIFY(job.canStart());
}

#include "moc_deletemessagejobtest.cpp"
