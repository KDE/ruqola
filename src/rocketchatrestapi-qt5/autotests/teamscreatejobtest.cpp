/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "teamscreatejobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamscreatejob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(TeamsCreateJobTest)
using namespace RocketChatRestApi;
TeamsCreateJobTest::TeamsCreateJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamsCreateJobTest::shouldHaveDefaultValue()
{
    TeamsCreateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    TeamsCreateJob::TeamsCreateJobInfo info = job.teamsCreateJobInfo();
    QVERIFY(!info.mReadOnly);
    QVERIFY(info.mTeamName.isEmpty());
    QVERIFY(info.mMembers.isEmpty());
    QVERIFY(info.mPassword.isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void TeamsCreateJobTest::shouldGenerateRequest()
{
    TeamsCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.create")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void TeamsCreateJobTest::shouldGenerateJson()
{
    TeamsCreateJob job;
    TeamsCreateJob::TeamsCreateJobInfo info;
    const QString channelname = QStringLiteral("foo1");
    info.mTeamName = channelname;
    job.setTeamsCreateJobInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"name":"%1"})").arg(channelname).toLatin1());

    bool readOnly = false;
    info.mReadOnly = readOnly;
    job.setTeamsCreateJobInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"name":"%1"})").arg(channelname).toLatin1());

    readOnly = true;
    info.mReadOnly = readOnly;
    job.setTeamsCreateJobInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"name":"%1","readOnly":true})").arg(channelname).toLatin1());

    const QStringList members = {QStringLiteral("foo"), QStringLiteral("bla")};
    info.mMembers = members;
    job.setTeamsCreateJobInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"members":["foo","bla"],"name":"%1","readOnly":true})").arg(channelname).toLatin1());
}

void TeamsCreateJobTest::shouldNotStarting()
{
    TeamsCreateJob job;

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

    TeamsCreateJob::TeamsCreateJobInfo info;
    const QString roomId = QStringLiteral("foo1");
    info.mTeamName = roomId;
    job.setTeamsCreateJobInfo(info);
    QVERIFY(job.canStart());
}
