/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "followmessagejobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/followmessagejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(FollowMessageJobTest)
using namespace RocketChatRestApi;
FollowMessageJobTest::FollowMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void FollowMessageJobTest::shouldHaveDefaultValue()
{
    FollowMessageJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void FollowMessageJobTest::shouldGenerateRequest()
{
    FollowMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.followMessage"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void FollowMessageJobTest::shouldGenerateJson()
{
    FollowMessageJob job;
    const QByteArray messageid("foo1");
    job.setMessageId(messageid);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"mid":"%1"})").arg(QLatin1StringView(messageid)).toLatin1());
}

void FollowMessageJobTest::shouldNotStarting()
{
    FollowMessageJob job;

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
    QVERIFY(!job.canStart());
    const QByteArray messageId("foo1");
    job.setMessageId(messageId);
    QVERIFY(job.canStart());
}

#include "moc_followmessagejobtest.cpp"
