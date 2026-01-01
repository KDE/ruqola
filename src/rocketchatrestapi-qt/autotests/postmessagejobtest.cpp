/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "postmessagejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/postmessagejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(PostMessageJobTest)
using namespace RocketChatRestApi;
PostMessageJobTest::PostMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void PostMessageJobTest::shouldHaveDefaultValue()
{
    PostMessageJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomIds().isEmpty());
    QVERIFY(job.text().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void PostMessageJobTest::shouldGenerateRequest()
{
    PostMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.postMessage"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void PostMessageJobTest::shouldGenerateJson()
{
    PostMessageJob job;
    const QByteArray roomId("foo1");
    const QString text = u"topic1"_s;
    job.setRoomIds({roomId});
    job.setText(text);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1","text":"%2"})").arg(QLatin1StringView(roomId), text).toLatin1());
}

void PostMessageJobTest::shouldNotStarting()
{
    PostMessageJob job;

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
    job.setRoomIds({roomId});
    QVERIFY(!job.canStart());
    const QString text = u"topic1"_s;
    job.setText(text);
    QVERIFY(job.canStart());
}

#include "moc_postmessagejobtest.cpp"
