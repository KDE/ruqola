/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reactonmessagejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/reactonmessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ReactOnMessageJobTest)
using namespace RocketChatRestApi;
ReactOnMessageJobTest::ReactOnMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void ReactOnMessageJobTest::shouldHaveDefaultValue()
{
    ReactOnMessageJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
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
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.react"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ReactOnMessageJobTest::shouldGenerateJson()
{
    ReactOnMessageJob job;
    const QByteArray messageid("foo1");
    const QString emoji = u"topic1"_s;
    job.setMessageId(messageid);
    job.setEmoji(emoji);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"emoji":"%1","messageId":"%2","shouldReact":true})").arg(emoji, QLatin1StringView(messageid)).toLatin1());

    job.setShouldReact(false);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"emoji":"%1","messageId":"%2","shouldReact":false})").arg(emoji, QLatin1StringView(messageid)).toLatin1());
}

void ReactOnMessageJobTest::shouldNotStarting()
{
    ReactOnMessageJob job;

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
    const QByteArray messageId("foo1");
    job.setMessageId(messageId);
    QVERIFY(!job.canStart());
    const QString emoji = u"topic1"_s;
    job.setEmoji(emoji);
    QVERIFY(job.canStart());
}

#include "moc_reactonmessagejobtest.cpp"
