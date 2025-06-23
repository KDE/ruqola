/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeothertokensjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "users/removeothertokensjob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(RemoveOtherTokensJobTest)
using namespace RocketChatRestApi;
RemoveOtherTokensJobTest::RemoveOtherTokensJobTest(QObject *parent)
    : QObject(parent)
{
}

void RemoveOtherTokensJobTest::shouldHaveDefaultValue()
{
    RemoveOtherTokensJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RemoveOtherTokensJobTest::shouldGenerateRequest()
{
    RemoveOtherTokensJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.removeOtherTokens"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RemoveOtherTokensJobTest::shouldGenerateJson()
{
    RemoveOtherTokensJob job;
    QVERIFY(job.json().toJson(QJsonDocument::Compact).isNull());
}

void RemoveOtherTokensJobTest::shouldNotStarting()
{
    RemoveOtherTokensJob job;

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

#include "moc_removeothertokensjobtest.cpp"
