/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "subscriptiongetonejobtest.h"

#include "restapimethod.h"
#include "subscriptions/subscriptiongetonejob.h"
#include <QTest>

QTEST_GUILESS_MAIN(SubscriptionGetOneJobTest)
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SubscriptionGetOneJobTest::SubscriptionGetOneJobTest(QObject *parent)
    : QObject(parent)
{
}

void SubscriptionGetOneJobTest::shouldHaveDefaultValue()
{
    SubscriptionGetOneJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.roomId().isEmpty());
}

void SubscriptionGetOneJobTest::shouldGenerateRequest()
{
    SubscriptionGetOneJob job;
    const QByteArray roomId{"foo"_ba};
    job.setRoomId(roomId);
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/subscriptions.getOne?roomId=%1"_s.arg(QString::fromLatin1(roomId))));
}

#include "moc_subscriptiongetonejobtest.cpp"
