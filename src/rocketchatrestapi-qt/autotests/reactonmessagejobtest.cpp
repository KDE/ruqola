/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reactonmessagejobtest.h"
#include "chat/reactonmessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ReactOnMessageJobTest)
using namespace RocketChatRestApi;
ReactOnMessageJobTest::ReactOnMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void ReactOnMessageJobTest::shouldHaveDefaultValue()
{
    ReactOnMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(job.emoji().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void ReactOnMessageJobTest::shouldGenerateRequest()
{
    ReactOnMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.react")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ReactOnMessageJobTest::shouldGenerateJson()
{
    ReactOnMessageJob job;
    const QString messageid = QStringLiteral("foo1");
    const QString emoji = QStringLiteral("topic1");
    job.setMessageId(messageid);
    job.setEmoji(emoji);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"emoji":"%1","messageId":"%2","shouldReact":true})").arg(emoji, messageid).toLatin1());

    job.setShouldReact(false);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"emoji":"%1","messageId":"%2","shouldReact":false})").arg(emoji, messageid).toLatin1());
}

void ReactOnMessageJobTest::shouldNotStarting()
{
    ReactOnMessageJob job;

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
    const QString messageId = QStringLiteral("foo1");
    job.setMessageId(messageId);
    QVERIFY(!job.canStart());
    const QString emoji = QStringLiteral("topic1");
    job.setEmoji(emoji);
    QVERIFY(job.canStart());
}

#include "moc_reactonmessagejobtest.cpp"
