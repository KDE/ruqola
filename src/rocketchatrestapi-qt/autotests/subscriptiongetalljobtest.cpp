/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "subscriptiongetalljobtest.h"

#include "restapimethod.h"
#include "subscriptions/subscriptiongetalljob.h"
#include <QTest>

QTEST_GUILESS_MAIN(SubscriptionGetAllJobTest)
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SubscriptionGetAllJobTest::SubscriptionGetAllJobTest(QObject *parent)
    : QObject(parent)
{
}

void SubscriptionGetAllJobTest::shouldHaveDefaultValue()
{
    SubscriptionGetAllJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
}

void SubscriptionGetAllJobTest::shouldGenerateRequest()
{
    SubscriptionGetAllJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/subscriptions.get"_s));
}

#include "moc_subscriptiongetalljobtest.cpp"
