/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/oauthappsjob.h"

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include <QTest>

QTEST_GUILESS_MAIN(OauthAppsJobTest)
using namespace RocketChatRestApi;
OauthAppsJobTest::OauthAppsJobTest(QObject *parent)
    : QObject(parent)
{
}

void OauthAppsJobTest::shouldHaveDefaultValue()
{
    OauthAppsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void OauthAppsJobTest::shouldGenerateRequest()
{
    OauthAppsJob job;
    const QString clientId = u"cli"_s;
    job.setClientId(clientId);

    const QString appId = u"appId"_s;
    job.setAppId(appId);
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/oauth-apps.get?clientId=%1&appId=%2"_s.arg(clientId, appId)));
}

void OauthAppsJobTest::shouldNotStarting()
{
    OauthAppsJob job;

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

    const QString clientId = u"cli"_s;
    job.setClientId(clientId);
    QVERIFY(!job.canStart());

    const QString appId = u"appId"_s;
    job.setAppId(appId);
    QVERIFY(job.canStart());
}

#include "moc_oauthappsjobtest.cpp"
