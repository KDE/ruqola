/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getusernamesuggestionjobtest.h"
#include "restapimethod.h"
#include "users/getusernamesuggestionjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(GetUsernameSuggestionJobTest)
using namespace RocketChatRestApi;
GetUsernameSuggestionJobTest::GetUsernameSuggestionJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetUsernameSuggestionJobTest::shouldHaveDefaultValue()
{
    GetUsernameSuggestionJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GetUsernameSuggestionJobTest::shouldGenerateRequest()
{
    GetUsernameSuggestionJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.getUsernameSuggestion")));
}

#include "moc_getusernamesuggestionjobtest.cpp"
