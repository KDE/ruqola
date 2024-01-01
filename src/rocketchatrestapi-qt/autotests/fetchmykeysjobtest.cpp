/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fetchmykeysjobtest.h"
#include "e2e/fetchmykeysjob.h"
#include "restapimethod.h"
#include <QTest>
QTEST_GUILESS_MAIN(FetchMyKeysJobTest)
using namespace RocketChatRestApi;
FetchMyKeysJobTest::FetchMyKeysJobTest(QObject *parent)
    : QObject(parent)
{
}

void FetchMyKeysJobTest::shouldHaveDefaultValue()
{
    FetchMyKeysJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void FetchMyKeysJobTest::shouldGenerateRequest()
{
    FetchMyKeysJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/e2e.fetchMyKeys")));
}

#include "moc_fetchmykeysjobtest.cpp"
