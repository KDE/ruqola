/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "googleauthjobtest.h"
#include "authentication/googleauthjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(GoogleAuthJobTest)
using namespace RocketChatRestApi;
GoogleAuthJobTest::GoogleAuthJobTest(QObject *parent)
    : QObject(parent)
{
}

void GoogleAuthJobTest::shouldHaveDefaultValue()
{
    GoogleAuthJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(job.idToken().isEmpty());
    QVERIFY(job.accessToken().isEmpty());
    QCOMPARE(job.expireTokenInSeconds(), -1);
    QVERIFY(!job.hasQueryParameterSupport());
}

void GoogleAuthJobTest::shouldGenerateRequest()
{
    GoogleAuthJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/login")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void GoogleAuthJobTest::shouldGenerateJson()
{
    GoogleAuthJob job;
    const QString secret = QStringLiteral("secret:");
    job.setIdToken(secret);

    const QString accessToken = QStringLiteral("accessToken");
    job.setAccessToken(accessToken);

    const int expireToken = 300;
    job.setExpireTokenInSeconds(expireToken);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"accessToken":"%1","expiresIn":300,"idToken":"%2","serviceName":"google"})").arg(accessToken, secret).toLatin1());
}

void GoogleAuthJobTest::shouldNotStarting()
{
    GoogleAuthJob job;

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
    job.setIdToken(secret);
    QVERIFY(!job.canStart());

    const QString accessToken = QStringLiteral("accessToken");
    job.setAccessToken(accessToken);
    QVERIFY(!job.canStart());

    const int expireToken = 300;
    job.setExpireTokenInSeconds(expireToken);
    QVERIFY(job.canStart());
}

#include "moc_googleauthjobtest.cpp"
