/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changearchivationstatejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "rooms/changearchivationstatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChangeArchivationStateJobTest)
using namespace RocketChatRestApi;
ChangeArchivationStateJobTest::ChangeArchivationStateJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeArchivationStateJobTest::shouldHaveDefaultValue()
{
    ChangeArchivationStateJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeArchivationStateJobTest::shouldGenerateRequest()
{
    ChangeArchivationStateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.changeArchivationState"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChangeArchivationStateJobTest::shouldGenerateJson()
{
    ChangeArchivationStateJob job;
    const QString roomId = u"foo1"_s;
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
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_changearchivationstatejobtest.cpp"
