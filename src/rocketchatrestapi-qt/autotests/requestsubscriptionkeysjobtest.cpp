/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "requestsubscriptionkeysjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "e2e/requestsubscriptionkeysjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(RequestSubscriptionKeysJobTest)
using namespace RocketChatRestApi;
RequestSubscriptionKeysJobTest::RequestSubscriptionKeysJobTest(QObject *parent)
    : QObject(parent)
{
}

void RequestSubscriptionKeysJobTest::shouldHaveDefaultValue()
{
    RequestSubscriptionKeysJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RequestSubscriptionKeysJobTest::shouldGenerateRequest()
{
    RequestSubscriptionKeysJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/e2e.requestSubscriptionKeys"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RequestSubscriptionKeysJobTest::shouldGenerateJson()
{
    RequestSubscriptionKeysJob job;
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), "{}"_ba);
}

void RequestSubscriptionKeysJobTest::shouldNotStarting()
{
    RequestSubscriptionKeysJob job;

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

#include "moc_requestsubscriptionkeysjobtest.cpp"
