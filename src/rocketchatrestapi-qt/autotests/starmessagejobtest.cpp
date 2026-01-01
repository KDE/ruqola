/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "starmessagejobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    QVERIFY(!job.canStart());
    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    job.setMessageId("bla"_ba);
    QVERIFY(job.canStart());
}

void StarMessageJobTest::shouldGenerateStarMessageRequest()
{
    StarMessageJob job;
    RestApiMethod method;
    const QString authToken = u"foo"_s;
    const QString userId = u"user"_s;
    job.setUserId(userId);
    job.setAuthToken(authToken);
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QByteArray messageId("foo");
    job.setMessageId(messageId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.starMessage"_s));
    QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
    QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
    QCOMPARE(request.rawHeader("X-Auth-Token"_ba), authToken.toLocal8Bit());
    QCOMPARE(request.rawHeader("X-User-Id"_ba), userId.toLocal8Bit());
}

void StarMessageJobTest::shouldGenerateUnStarMessageRequest()
{
    StarMessageJob job;
    job.setStarMessage(false);
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QByteArray messageId("foo");
    job.setMessageId(messageId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.unStarMessage"_s));
    QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
    QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void StarMessageJobTest::shouldGenerateJson()
{
    StarMessageJob job;
    const QByteArray messageId("foo1");
    job.setMessageId(messageId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"messageId":"%1"})").arg(QLatin1StringView(messageId)).toLatin1());
}

#include "moc_starmessagejobtest.cpp"
