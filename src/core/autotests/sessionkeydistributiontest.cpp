/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sessionkeydistributiontest.h"
#include "e2e/provideuserswithsuggestedgroupkeysjob.h"
#include "restapimethod.h"
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QTest>
QTEST_GUILESS_MAIN(SessionKeyDistributionTest)
SessionKeyDistributionTest::SessionKeyDistributionTest(QObject *parent)
    : QObject(parent)
{
}

void SessionKeyDistributionTest::testJsonPayload()
{
    RocketChatRestApi::ProvideUsersWithSuggestedGroupKeysJob job;
    job.setRoomId(QStringLiteral("123"));
    const QVector<RocketChatRestApi::SuggestedGroupKey> suggestedGroupKeys = {{QStringLiteral("users"), QStringLiteral("base64keyA")},
                                                                              {QStringLiteral("userB"), QStringLiteral("base64keyB")}};
    job.setKeys(suggestedGroupKeys);

    const QJsonDocument doc = job.json();
    const QJsonObject obj = doc.object();
    const QJsonObject usersSuggestedGroupKeys = obj[QStringLiteral("usersSuggestedGroupKeys")].toObject();
    QJsonArray arr = usersSuggestedGroupKeys[QStringLiteral("123")].toArray();
    QCOMPARE(arr.size(), 2);
    QCOMPARE(arr[0].toObject()[QStringLiteral("_id")].toString(), QStringLiteral("users"));
    QCOMPARE(arr[0].toObject()[QStringLiteral("key")].toString(), QStringLiteral("base64keyA"));
}

void SessionKeyDistributionTest::testCanStartValidation()
{
    RocketChatRestApi::ProvideUsersWithSuggestedGroupKeysJob job;
    const auto networkManager = new QNetworkAccessManager(this);
    job.setNetworkAccessManager(networkManager);
    job.setAuthToken(QStringLiteral("dummyToken"));
    job.setUserId(QStringLiteral("dummyUserId"));
    const auto restApiMethod = new RocketChatRestApi::RestApiMethod;
    restApiMethod->setServerUrl(QStringLiteral("http://localhost:3000"));
    job.setRestApiMethod(restApiMethod);
    QVERIFY(!job.canStart());

    job.setRoomId(QStringLiteral("room123"));
    QVERIFY(!job.canStart());

    const QVector<RocketChatRestApi::SuggestedGroupKey> keys = {{QStringLiteral("users"), QStringLiteral("base64keyA")}};
    job.setKeys(keys);
    QVERIFY(job.canStart());
    delete restApiMethod;
}

#include "moc_sessionkeydistributiontest.cpp"
