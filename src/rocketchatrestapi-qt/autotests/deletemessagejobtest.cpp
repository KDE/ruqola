/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deletemessagejobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/deletemessagejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(DeleteMessageJobTest)
using namespace RocketChatRestApi;
DeleteMessageJobTest::DeleteMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void DeleteMessageJobTest::shouldHaveDefaultValue()
{
    DeleteMessageJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void DeleteMessageJobTest::shouldGenerateRequest()
{
    DeleteMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.delete"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void DeleteMessageJobTest::shouldGenerateJson()
{
    DeleteMessageJob job;
    const QByteArray roomId("foo1");
    const QByteArray text("topic1");
    job.setRoomId(roomId);
    job.setMessageId(text);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"msgId":"%2","roomId":"%1"})").arg(QLatin1StringView(roomId), QLatin1StringView(text)).toLatin1());
}

void DeleteMessageJobTest::shouldNotStarting()
{
    DeleteMessageJob job;

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
    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    QVERIFY(!job.canStart());
    const QByteArray text("topic1");
    job.setMessageId(text);
    QVERIFY(job.canStart());
}

#include "moc_deletemessagejobtest.cpp"
