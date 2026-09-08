/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "unfollowmessagejobtest.h"

#include "chat/unfollowmessagejob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(UnFollowMessageJobTest)
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UnFollowMessageJobTest::UnFollowMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void UnFollowMessageJobTest::shouldHaveDefaultValue()
{
    UnFollowMessageJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void UnFollowMessageJobTest::shouldGenerateRequest()
{
    UnFollowMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.unfollowMessage"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void UnFollowMessageJobTest::shouldGenerateJson()
{
    UnFollowMessageJob job;
    const QByteArray messageid("foo1");
    job.setMessageId(messageid);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"mid":"%1"})"_s.arg(QLatin1StringView(messageid)).toLatin1());
}

void UnFollowMessageJobTest::shouldNotStarting()
{
    UnFollowMessageJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QByteArray messageId("foo1");
    job.setMessageId(messageId);
    QVERIFY(job.canStart());
}

#include "moc_unfollowmessagejobtest.cpp"
