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
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(SessionKeyDistributionTest)
SessionKeyDistributionTest::SessionKeyDistributionTest(QObject *parent)
    : QObject(parent)
{
}

void SessionKeyDistributionTest::testJsonPayload()
{
    RocketChatRestApi::ProvideUsersWithSuggestedGroupKeysJob job;
    job.setRoomId(u"123"_s);
    const QVector<RocketChatRestApi::SuggestedGroupKey> suggestedGroupKeys = {{u"users"_s, u"base64keyA"_s}, {u"userB"_s, u"base64keyB"_s}};
    job.setKeys(suggestedGroupKeys);

    const QJsonDocument doc = job.json();
    const QJsonObject obj = doc.object();
    const QJsonObject usersSuggestedGroupKeys = obj[u"usersSuggestedGroupKeys"_s].toObject();
    QJsonArray arr = usersSuggestedGroupKeys[u"123"_s].toArray();
    QCOMPARE(arr.size(), 2);
    QCOMPARE(arr[0].toObject()[u"_id"_s].toString(), u"users"_s);
    QCOMPARE(arr[0].toObject()[u"key"_s].toString(), u"base64keyA"_s);
}

void SessionKeyDistributionTest::testCanStartValidation()
{
    RocketChatRestApi::ProvideUsersWithSuggestedGroupKeysJob job;
    const auto networkManager = new QNetworkAccessManager(this);
    job.setNetworkAccessManager(networkManager);
    job.setAuthToken(u"dummyToken"_s);
    job.setUserId(u"dummyUserId"_s);
    const auto restApiMethod = new RocketChatRestApi::RestApiMethod;
    restApiMethod->setServerUrl(u"http://localhost:3000"_s);
    job.setRestApiMethod(restApiMethod);
    QVERIFY(!job.canStart());

    job.setRoomId(u"room123"_s);
    QVERIFY(!job.canStart());

    const QVector<RocketChatRestApi::SuggestedGroupKey> keys = {{u"users"_s, u"base64keyA"_s}};
    job.setKeys(keys);
    QVERIFY(job.canStart());
    delete restApiMethod;
}

#include "moc_sessionkeydistributiontest.cpp"
