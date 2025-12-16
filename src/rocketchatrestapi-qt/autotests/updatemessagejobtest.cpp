/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "updatemessagejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/updatemessagejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(UpdateMessageJobTest)
using namespace RocketChatRestApi;
UpdateMessageJobTest::UpdateMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void UpdateMessageJobTest::shouldHaveDefaultValue()
{
    UpdateMessageJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(job.updatedText().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void UpdateMessageJobTest::shouldGenerateRequest()
{
    UpdateMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.update"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void UpdateMessageJobTest::shouldGenerateJson()
{
    UpdateMessageJob job;
    const QByteArray roomId("foo1");
    const QByteArray messageId("topic1");
    const QString updatedText = u"topic1"_s;
    job.setRoomId(roomId);
    job.setMessageId(messageId);
    job.setUpdatedText(updatedText);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(R"({"msgId":"%2","roomId":"%1","text":"%3"})").arg(QLatin1StringView(roomId), QLatin1StringView(messageId), updatedText).toLatin1());
}

void UpdateMessageJobTest::shouldNotStarting()
{
    UpdateMessageJob job;

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
    QVERIFY(job.updatedText().isEmpty());
    QVERIFY(job.canStart());
    job.setUpdatedText(u"too"_s);
    QVERIFY(job.canStart());
}

#include "moc_updatemessagejobtest.cpp"
