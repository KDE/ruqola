/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getbannersjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "banner/getbannersjob.h"
#include "restapimethod.h"
#include <QTest>

QTEST_GUILESS_MAIN(GetBannersJobTest)
using namespace RocketChatRestApi;
GetBannersJobTest::GetBannersJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetBannersJobTest::shouldHaveDefaultValue()
{
    GetBannersJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void GetBannersJobTest::shouldGenerateRequest()
{
    GetBannersJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/banners?platform=web"_s));
}

void GetBannersJobTest::shouldNotStarting()
{
    GetBannersJob job;

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

    QVERIFY(job.canStart());
}

#include "moc_getbannersjobtest.cpp"
