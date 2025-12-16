/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "twitterauthjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "authentication/twitterauthjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(TwitterAuthJobTest)
using namespace RocketChatRestApi;
TwitterAuthJobTest::TwitterAuthJobTest(QObject *parent)
    : QObject(parent)
{
}

void TwitterAuthJobTest::shouldHaveDefaultValue()
{
    TwitterAuthJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(job.secret().isEmpty());
    QVERIFY(job.accessToken().isEmpty());
    QCOMPARE(job.expireTokenInSeconds(), -1);
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void TwitterAuthJobTest::shouldGenerateRequest()
{
    TwitterAuthJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/login"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void TwitterAuthJobTest::shouldGenerateJson()
{
    TwitterAuthJob job;
    const QString secret = u"secret:"_s;
    job.setSecret(secret);

    const QString accessToken = u"accessToken"_s;
    job.setAccessToken(accessToken);

    const QString appId = u"appid"_s;
    job.setAppId(appId);

    const QString appSecret = u"appSecret"_s;
    job.setAppSecret(appSecret);

    const int expireToken = 300;
    job.setExpireTokenInSeconds(expireToken);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"accessToken":"%1","accessTokenSecret":"%2","appId":"%3","appSecret":"%4","expiresIn":300,"serviceName":"twitter"})")
                 .arg(accessToken, secret, appId, appSecret)
                 .toLatin1());
}

void TwitterAuthJobTest::shouldNotStarting()
{
    TwitterAuthJob job;

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
    const QString secret = u"secret:"_s;
    job.setSecret(secret);
    QVERIFY(!job.canStart());

    const QString accessToken = u"accessToken"_s;
    job.setAccessToken(accessToken);
    QVERIFY(!job.canStart());

    const int expireToken = 300;
    job.setExpireTokenInSeconds(expireToken);
    QVERIFY(!job.canStart());

    const QString appId = u"appid"_s;
    job.setAppId(appId);
    QVERIFY(!job.canStart());

    const QString appSecret = u"appSecret"_s;
    job.setAppSecret(appSecret);
    QVERIFY(job.canStart());
}

#include "moc_twitterauthjobtest.cpp"
