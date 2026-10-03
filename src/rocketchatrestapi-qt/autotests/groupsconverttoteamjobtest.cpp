/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupsconverttoteamjobtest.h"
using namespace Qt::Literals::StringLiterals;

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupsConvertToTeamJobTest::shouldGenerateRequest()
{
    GroupsConvertToTeamJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.convertToTeam"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void GroupsConvertToTeamJobTest::shouldGenerateJson()
{
    GroupsConvertToTeamJob job;
    const QString roomId = u"foo2"_s;
    job.setRoomId(roomId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"roomId":"%1"})"_s.arg(roomId).toLatin1());
}

void GroupsConvertToTeamJobTest::shouldNotStarting()
{
    GroupsConvertToTeamJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QString roomId = u"foo2"_s;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_groupsconverttoteamjobtest.cpp"
