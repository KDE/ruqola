/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sendmessagejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/sendmessagejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(SendMessageJobTest)
using namespace RocketChatRestApi;
SendMessageJobTest::SendMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void SendMessageJobTest::shouldHaveDefaultValue()
{
    SendMessageJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    SendMessageJob::SendMessageArguments args = job.sendMessageArguments();
    QVERIFY(args.roomId.isEmpty());
    QVERIFY(args.message.isEmpty());
    QVERIFY(args.threadMessageId.isEmpty());
    QVERIFY(args.messageId.isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SendMessageJobTest::shouldGenerateRequest()
{
    SendMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.sendMessage"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SendMessageJobTest::shouldGenerateJson()
{
    SendMessageJob job;
    const QString roomId = u"foo1"_s;
    const QString text = u"topic1"_s;
    SendMessageJob::SendMessageArguments args;
    args.roomId = roomId;
    args.message = text;
    job.setSendMessageArguments(args);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"message":{"msg":"%2","rid":"%1"}})").arg(roomId, text).toLatin1());

    const QString threadId = u"threadid"_s;
    args.threadMessageId = threadId;
    job.setSendMessageArguments(args);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"message":{"msg":"%2","rid":"%1","tmid":"%3"}})").arg(roomId, text, threadId).toLatin1());

    const QString messageId = u"msgid"_s;
    args.messageId = messageId;
    job.setSendMessageArguments(args);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"message":{"_id":"%4","msg":"%2","rid":"%1","tmid":"%3"}})").arg(roomId, text, threadId, messageId).toLatin1());
}

void SendMessageJobTest::shouldNotStarting()
{
    SendMessageJob job;

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
    SendMessageJob::SendMessageArguments args;
    args.roomId = u"foo1"_s;
    job.setSendMessageArguments(args);
    QVERIFY(!job.canStart());

    args.message = u"topic1"_s;
    job.setSendMessageArguments(args);
    QVERIFY(job.canStart());
}

#include "moc_sendmessagejobtest.cpp"
