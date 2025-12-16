/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsuiinteractionjobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/appsuiinteractionjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(AppsUiInteractionJobTest)
using namespace RocketChatRestApi;
AppsUiInteractionJobTest::AppsUiInteractionJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppsUiInteractionJobTest::shouldHaveDefaultValue()
{
    AppsUiInteractionJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    AppsUiInteractionJob::AppsUiInteractionJobInfo info;
    job.setAppsUiInteractionJobInfo(info);
    QVERIFY(job.requireHttpAuthentication());
}

void AppsUiInteractionJobTest::shouldGenerateRequest()
{
    AppsUiInteractionJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/ui.interaction"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);

    AppsUiInteractionJob::AppsUiInteractionJobInfo info;
    info.methodName = u"login"_s;
    job.setAppsUiInteractionJobInfo(info);
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/ui.interaction/login"_s));
}

void AppsUiInteractionJobTest::shouldGenerateJson()
{
    AppsUiInteractionJob job;
    AppsUiInteractionJob::AppsUiInteractionJobInfo info;
    info.methodName = u"login"_s;
    job.setAppsUiInteractionJobInfo(info);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({})").toLatin1());

    QVariantMap map;
    map.insert(u"msg"_s, u"method"_s);
    map.insert(u"id"_s, u"52"_s);
    map.insert(u"method"_s, u"login"_s);
    info.messageObj = QJsonObject::fromVariantMap(map);
    info.methodName = u"login"_s;
    job.setAppsUiInteractionJobInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), u"{\"id\":\"52\",\"method\":\"login\",\"msg\":\"method\"}"_s.toLatin1());
}

void AppsUiInteractionJobTest::shouldNotStarting()
{
    AppsUiInteractionJob job;

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
    // TODO
}

void AppsUiInteractionJobTest::shouldTestGenerateMessageObj()
{
    QFETCH(QString, actionId);
    QFETCH(QString, value);
    QFETCH(QString, blockId);
    QFETCH(QByteArray, roomId);
    QFETCH(QByteArray, messageId);
    QFETCH(QString, result);

    AppsUiInteractionJob::AppsUiInteractionJobInfo info;
    info.generateMessageObj(actionId, value, blockId, roomId, messageId);
    QCOMPARE(QString::fromUtf8(QJsonDocument(info.messageObj).toJson(QJsonDocument::Compact)), result);
}

void AppsUiInteractionJobTest::shouldTestGenerateMessageObj_data()
{
    QTest::addColumn<QString>("actionId");
    QTest::addColumn<QString>("value");
    QTest::addColumn<QString>("blockId");
    QTest::addColumn<QByteArray>("roomId");
    QTest::addColumn<QByteArray>("messageId");
    QTest::addColumn<QString>("result");

    QTest::addRow("empty") << QString() << QString() << QString() << QByteArray() << QByteArray()
                           << QStringLiteral(
                                  "{\"actionId\":\"\",\"container\":{\"id\":\"\",\"type\":\"message\"},\"mid\":\"\",\"payload\":{\"blockId\":\"\",\"value\":"
                                  "\"\"},\"rid\":\"\",\"triggerId\":\"foo\",\"type\":\"blockAction\"}");

    QTest::addRow("test1") << u"act1"_s << QString() << u"blo1"_s << QByteArrayLiteral("room1") << "message1"_ba
                           << QStringLiteral(
                                  "{\"actionId\":\"act1\",\"container\":{\"id\":\"message1\",\"type\":\"message\"},\"mid\":\"message1\",\"payload\":{"
                                  "\"blockId\":\"blo1\",\"value\":\"\"},\"rid\":\"room1\",\"triggerId\":\"foo\",\"type\":\"blockAction\"}");

    QTest::addRow("test2") << u"act1"_s << u"[{\"_id\":\"HJ4EFjvEjYT73X\",\"username\":\"service\",\"name\":\"Service\",\"type\":\"user\"}]"_s << u"blo1"_s
                           << QByteArrayLiteral("room1") << "message1"_ba
                           << QStringLiteral(
                                  "{\"actionId\":\"act1\",\"container\":{\"id\":\"message1\",\"type\":\"message\"},\"mid\":\"message1\",\"payload\":{"
                                  "\"blockId\":\"blo1\",\"value\":\"[{\\\"_id\\\":\\\"HJ4EFjvEjYT73X\\\",\\\"username\\\":\\\"service\\\",\\\"name\\\":"
                                  "\\\"Service\\\",\\\"type\\\":\\\"user\\\"}]\"},\"rid\":\"room1\",\"triggerId\":\"foo\",\"type\":\"blockAction\"}");
}

#include "moc_appsuiinteractionjobtest.cpp"
