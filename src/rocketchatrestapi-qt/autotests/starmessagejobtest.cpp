/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "starmessagejobtest.h"
#include "chat/starmessagejob.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(StarMessageJobTest)
using namespace RocketChatRestApi;
StarMessageJobTest::StarMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void StarMessageJobTest::shouldHaveDefaultValue()
{
    StarMessageJob job;
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.start());
    QVERIFY(job.authToken().isEmpty());
    QVERIFY(job.authCode().isEmpty());
    QVERIFY(job.authMethod().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.starMessage());
    QVERIFY(!job.hasQueryParameterSupport());
}

void StarMessageJobTest::shouldHaveMessageId()
{
    StarMessageJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    QVERIFY(!job.canStart());
    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    job.setMessageId(QStringLiteral("bla"));
    QVERIFY(job.canStart());
}

void StarMessageJobTest::shouldGenerateStarMessageRequest()
{
    StarMessageJob job;
    RestApiMethod method;
    const QString authToken = QStringLiteral("foo");
    const QString userId = QStringLiteral("user");
    job.setUserId(userId);
    job.setAuthToken(authToken);
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString messageId = QStringLiteral("foo");
    job.setMessageId(messageId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.starMessage")));
    QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
    QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
    QCOMPARE(request.rawHeader(QByteArrayLiteral("X-Auth-Token")), authToken.toLocal8Bit());
    QCOMPARE(request.rawHeader(QByteArrayLiteral("X-User-Id")), userId.toLocal8Bit());
}

void StarMessageJobTest::shouldGenerateUnStarMessageRequest()
{
    StarMessageJob job;
    job.setStarMessage(false);
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString messageId = QStringLiteral("foo");
    job.setMessageId(messageId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.unStarMessage")));
    QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
    QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void StarMessageJobTest::shouldGenerateJson()
{
    StarMessageJob job;
    const QString messageId = QStringLiteral("foo1");
    job.setMessageId(messageId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"messageId":"%1"})").arg(messageId).toLatin1());
}

#include "moc_starmessagejobtest.cpp"
