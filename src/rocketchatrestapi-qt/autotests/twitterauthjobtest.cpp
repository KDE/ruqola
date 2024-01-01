/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "twitterauthjobtest.h"
#include "authentication/twitterauthjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(TwitterAuthJobTest)
using namespace RocketChatRestApi;
TwitterAuthJobTest::TwitterAuthJobTest(QObject *parent)
    : QObject(parent)
{
}

void TwitterAuthJobTest::shouldHaveDefaultValue()
{
    TwitterAuthJob job;
    verifyDefaultValue(&job);
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
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/login")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void TwitterAuthJobTest::shouldGenerateJson()
{
    TwitterAuthJob job;
    const QString secret = QStringLiteral("secret:");
    job.setSecret(secret);

    const QString accessToken = QStringLiteral("accessToken");
    job.setAccessToken(accessToken);

    const QString appId = QStringLiteral("appid");
    job.setAppId(appId);

    const QString appSecret = QStringLiteral("appSecret");
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
    const QString secret = QStringLiteral("secret:");
    job.setSecret(secret);
    QVERIFY(!job.canStart());

    const QString accessToken = QStringLiteral("accessToken");
    job.setAccessToken(accessToken);
    QVERIFY(!job.canStart());

    const int expireToken = 300;
    job.setExpireTokenInSeconds(expireToken);
    QVERIFY(!job.canStart());

    const QString appId = QStringLiteral("appid");
    job.setAppId(appId);
    QVERIFY(!job.canStart());

    const QString appSecret = QStringLiteral("appSecret");
    job.setAppSecret(appSecret);
    QVERIFY(job.canStart());
}

#include "moc_twitterauthjobtest.cpp"
