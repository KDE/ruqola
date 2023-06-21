/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupsconverttoteamjobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/groupsconverttoteamjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(GroupsConvertToTeamJobTest)
using namespace RocketChatRestApi;
GroupsConvertToTeamJobTest::GroupsConvertToTeamJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupsConvertToTeamJobTest::shouldHaveDefaultValue()
{
    GroupsConvertToTeamJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupsConvertToTeamJobTest::shouldGenerateRequest()
{
    GroupsConvertToTeamJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.convertToTeam")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void GroupsConvertToTeamJobTest::shouldGenerateJson()
{
    GroupsConvertToTeamJob job;
    const QString roomId = QStringLiteral("foo2");
    job.setRoomId(roomId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(roomId).toLatin1());
}

void GroupsConvertToTeamJobTest::shouldNotStarting()
{
    GroupsConvertToTeamJob job;

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
    const QString roomId = QStringLiteral("foo2");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_groupsconverttoteamjobtest.cpp"
