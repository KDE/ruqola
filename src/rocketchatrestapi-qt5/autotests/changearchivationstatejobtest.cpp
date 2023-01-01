/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changearchivationstatejobtest.h"
#include "rooms/changearchivationstatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeArchivationStateJobTest)
using namespace RocketChatRestApi;
ChangeArchivationStateJobTest::ChangeArchivationStateJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeArchivationStateJobTest::shouldHaveDefaultValue()
{
    ChangeArchivationStateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeArchivationStateJobTest::shouldGenerateRequest()
{
    ChangeArchivationStateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.changeArchivationState")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeArchivationStateJobTest::shouldGenerateJson()
{
    ChangeArchivationStateJob job;
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    job.setArchive(true);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"action":"archive","rid":"%1"})").arg(roomId).toLatin1());
    job.setArchive(false);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"action":"unarchive","rid":"%1"})").arg(roomId).toLatin1());
}

void ChangeArchivationStateJobTest::shouldNotStarting()
{
    ChangeArchivationStateJob job;

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
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}
