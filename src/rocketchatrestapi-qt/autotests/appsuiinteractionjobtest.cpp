/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsuiinteractionjobtest.h"
#include "misc/appsuiinteractionjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(AppsUiInteractionJobTest)
using namespace RocketChatRestApi;
AppsUiInteractionJobTest::AppsUiInteractionJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppsUiInteractionJobTest::shouldHaveDefaultValue()
{
    AppsUiInteractionJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    AppsUiInteractionJob::AppsUiInteractionJobInfo info;
    job.setAppsUiInteractionJobInfo(info);
    QVERIFY(job.requireHttpAuthentication());
}

void AppsUiInteractionJobTest::shouldGenerateRequest()
{
    AppsUiInteractionJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/ui.interaction")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));

    AppsUiInteractionJob::AppsUiInteractionJobInfo info;
    info.methodName = QStringLiteral("login");
    job.setAppsUiInteractionJobInfo(info);
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/ui.interaction/login")));
}

void AppsUiInteractionJobTest::shouldGenerateJson()
{
    AppsUiInteractionJob job;
    AppsUiInteractionJob::AppsUiInteractionJobInfo info;
    info.methodName = QStringLiteral("login");
    job.setAppsUiInteractionJobInfo(info);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({})").toLatin1());

    QVariantMap map;
    map.insert(QStringLiteral("msg"), QStringLiteral("method"));
    map.insert(QStringLiteral("id"), QStringLiteral("52"));
    map.insert(QStringLiteral("method"), QStringLiteral("login"));
    info.messageObj = QJsonObject::fromVariantMap(map);
    info.methodName = QStringLiteral("login");
    job.setAppsUiInteractionJobInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"id\":\"52\",\"method\":\"login\",\"msg\":\"method\"}").toLatin1());
}

void AppsUiInteractionJobTest::shouldNotStarting()
{
    AppsUiInteractionJob job;

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

    QTest::addRow("test1") << QStringLiteral("act1") << QString() << QStringLiteral("blo1") << QByteArrayLiteral("room1") << "message1"_ba
                           << QStringLiteral(
                                  "{\"actionId\":\"act1\",\"container\":{\"id\":\"message1\",\"type\":\"message\"},\"mid\":\"message1\",\"payload\":{"
                                  "\"blockId\":\"blo1\",\"value\":\"\"},\"rid\":\"room1\",\"triggerId\":\"foo\",\"type\":\"blockAction\"}");

    QTest::addRow("test2") << QStringLiteral("act1")
                           << QStringLiteral("[{\"_id\":\"HJ4EFjvEjYT73X\",\"username\":\"service\",\"name\":\"Service\",\"type\":\"user\"}]")
                           << QStringLiteral("blo1") << QByteArrayLiteral("room1") << "message1"_ba
                           << QStringLiteral(
                                  "{\"actionId\":\"act1\",\"container\":{\"id\":\"message1\",\"type\":\"message\"},\"mid\":\"message1\",\"payload\":{"
                                  "\"blockId\":\"blo1\",\"value\":\"[{\\\"_id\\\":\\\"HJ4EFjvEjYT73X\\\",\\\"username\\\":\\\"service\\\",\\\"name\\\":"
                                  "\\\"Service\\\",\\\"type\\\":\\\"user\\\"}]\"},\"rid\":\"room1\",\"triggerId\":\"foo\",\"type\":\"blockAction\"}");
}

#include "moc_appsuiinteractionjobtest.cpp"
