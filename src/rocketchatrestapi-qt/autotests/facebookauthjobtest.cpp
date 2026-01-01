/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "facebookauthjobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "authentication/facebookauthjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(FacebookAuthJobTest)
using namespace RocketChatRestApi;
FacebookAuthJobTest::FacebookAuthJobTest(QObject *parent)
    : QObject(parent)
{
}

void FacebookAuthJobTest::shouldHaveDefaultValue()
{
    FacebookAuthJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(job.secret().isEmpty());
    QVERIFY(job.accessToken().isEmpty());
    QCOMPARE(job.expireTokenInSeconds(), -1);
    QVERIFY(!job.hasQueryParameterSupport());
}

void FacebookAuthJobTest::shouldGenerateRequest()
{
    FacebookAuthJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/login"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void FacebookAuthJobTest::shouldGenerateJson()
{
    FacebookAuthJob job;
    const QString secret = u"secret:"_s;
    job.setSecret(secret);

    const QString accessToken = u"accessToken"_s;
    job.setAccessToken(accessToken);

    const int expireToken = 300;
    job.setExpireTokenInSeconds(expireToken);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"accessToken":"%1","expiresIn":300,"secret":"%2","serviceName":"facebook"})").arg(accessToken, secret).toLatin1());
}

void FacebookAuthJobTest::shouldNotStarting()
{
    FacebookAuthJob job;

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
    QVERIFY(job.canStart());
}

#include "moc_facebookauthjobtest.cpp"
