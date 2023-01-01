/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "unfollowmessagejobtest.h"
#include "chat/unfollowmessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(UnFollowMessageJobTest)
using namespace RocketChatRestApi;
UnFollowMessageJobTest::UnFollowMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void UnFollowMessageJobTest::shouldHaveDefaultValue()
{
    UnFollowMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void UnFollowMessageJobTest::shouldGenerateRequest()
{
    UnFollowMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.unfollowMessage")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UnFollowMessageJobTest::shouldGenerateJson()
{
    UnFollowMessageJob job;
    const QString messageid = QStringLiteral("foo1");
    job.setMessageId(messageid);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"mid":"%1"})").arg(messageid).toLatin1());
}

void UnFollowMessageJobTest::shouldNotStarting()
{
    UnFollowMessageJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString messageId = QStringLiteral("foo1");
    job.setMessageId(messageId);
    QVERIFY(job.canStart());
}
