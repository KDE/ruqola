/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamdeletejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamdeletejob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(TeamDeleteJobTest)
using namespace RocketChatRestApi;
TeamDeleteJobTest::TeamDeleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamDeleteJobTest::shouldHaveDefaultValue()
{
    TeamDeleteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomsId().isEmpty());
    QVERIFY(job.teamId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void TeamDeleteJobTest::shouldGenerateRequest()
{
    TeamDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/teams.delete"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void TeamDeleteJobTest::shouldGenerateJson()
{
    TeamDeleteJob job;
    const QByteArray teamId("foo2");
    job.setTeamId(teamId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"teamId":"%1"})").arg(QLatin1StringView(teamId)).toLatin1());
    const QList<QByteArray> rooms = {QByteArrayLiteral("bla"), "bla1"_ba};
    job.setRoomsId(rooms);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"roomsToRemove":["bla","bla1"],"teamId":"%1"})").arg(QLatin1StringView(teamId)).toLatin1());
}

void TeamDeleteJobTest::shouldNotStarting()
{
    TeamDeleteJob job;

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
    QVERIFY(!job.canStart());
    const QByteArray teamId("foo2");
    job.setTeamId(teamId);
    QVERIFY(job.canStart());
    // roomsId can be empty
    const QList<QByteArray> rooms = {QByteArrayLiteral("bb"), "aa"_ba};
    job.setRoomsId(rooms);
    QVERIFY(job.canStart());
}

#include "moc_teamdeletejobtest.cpp"
