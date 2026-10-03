/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "followmessagejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/followmessagejob.h"
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
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"mid":"%1"})"_s.arg(QLatin1StringView(messageid)).toLatin1());
}

void FollowMessageJobTest::shouldNotStarting()
{
    FollowMessageJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QByteArray messageId("foo1");
    job.setMessageId(messageId);
    QVERIFY(job.canStart());
}

#include "moc_followmessagejobtest.cpp"
