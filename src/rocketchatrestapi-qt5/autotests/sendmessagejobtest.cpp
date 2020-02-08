/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "sendmessagejobtest.h"
#include "chat/sendmessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(SendMessageJobTest)
using namespace RocketChatRestApi;
SendMessageJobTest::SendMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void SendMessageJobTest::shouldHaveDefaultValue()
{
    SendMessageJob job;
    verifyDefaultValue(&job);
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
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.sendMessage")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void SendMessageJobTest::shouldGenerateJson()
{
    SendMessageJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString text = QStringLiteral("topic1");
    SendMessageJob::SendMessageArguments args;
    args.roomId = roomId;
    args.message = text;
    job.setSendMessageArguments(args);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"message\":{\"msg\":\"%2\",\"rid\":\"%1\"}}").arg(roomId, text).toLatin1());

    const QString threadId = QStringLiteral("threadid");
    args.threadMessageId = threadId;
    job.setSendMessageArguments(args);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"message\":{\"msg\":\"%2\",\"rid\":\"%1\",\"tmid\":\"%3\"}}").arg(roomId, text, threadId).toLatin1());

    const QString messageId = QStringLiteral("msgid");
    args.messageId = messageId;
    job.setSendMessageArguments(args);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"message\":{\"_id\":\"%4\",\"msg\":\"%2\",\"rid\":\"%1\",\"tmid\":\"%3\"}}").arg(roomId, text, threadId, messageId).toLatin1());
}

void SendMessageJobTest::shouldNotStarting()
{
    SendMessageJob job;

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
    SendMessageJob::SendMessageArguments args;
    args.roomId = QStringLiteral("foo1");
    job.setSendMessageArguments(args);
    QVERIFY(!job.canStart());

    args.message = QStringLiteral("topic1");
    job.setSendMessageArguments(args);
    QVERIFY(job.canStart());
}
