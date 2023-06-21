/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "updatemessagejobtest.h"
#include "chat/updatemessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(UpdateMessageJobTest)
using namespace RocketChatRestApi;
UpdateMessageJobTest::UpdateMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void UpdateMessageJobTest::shouldHaveDefaultValue()
{
    UpdateMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(job.updatedText().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void UpdateMessageJobTest::shouldGenerateRequest()
{
    UpdateMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.update")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UpdateMessageJobTest::shouldGenerateJson()
{
    UpdateMessageJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString messageId = QStringLiteral("topic1");
    const QString updatedText = QStringLiteral("topic1");
    job.setRoomId(roomId);
    job.setMessageId(messageId);
    job.setUpdatedText(updatedText);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"msgId":"%2","roomId":"%1","text":"%3"})").arg(roomId, messageId, updatedText).toLatin1());
}

void UpdateMessageJobTest::shouldNotStarting()
{
    UpdateMessageJob job;

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
    QVERIFY(job.updatedText().isEmpty());
    QVERIFY(job.canStart());
    job.setUpdatedText(QStringLiteral("too"));
    QVERIFY(job.canStart());
}

#include "moc_updatemessagejobtest.cpp"
