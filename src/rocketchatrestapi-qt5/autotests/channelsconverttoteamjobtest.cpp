/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "channelsconverttoteamjobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/channelsconverttoteamjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChannelsConvertToTeamJobTest)
using namespace RocketChatRestApi;
ChannelsConvertToTeamJobTest::ChannelsConvertToTeamJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelsConvertToTeamJobTest::shouldHaveDefaultValue()
{
    ChannelsConvertToTeamJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomIds().isEmpty());
    QVERIFY(job.teamId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelsConvertToTeamJobTest::shouldGenerateRequest()
{
    ChannelsConvertToTeamJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.convertToTeam")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChannelsConvertToTeamJobTest::shouldGenerateJson()
{
    ChannelsConvertToTeamJob job;
    const QString roomId = QStringLiteral("foo1");
    job.setRoomIds({roomId});
    const QString teamId = QStringLiteral("foo2");
    job.setTeamId(teamId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"rooms":["%1"],"teamId":"%2"})").arg(roomId).arg(teamId).toLatin1());
}

void ChannelsConvertToTeamJobTest::shouldNotStarting()
{
    ChannelsConvertToTeamJob job;

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
    job.setRoomIds({roomId});
    QVERIFY(!job.canStart());
    const QString teamId = QStringLiteral("foo2");
    job.setTeamId(teamId);
    QVERIFY(job.canStart());
}
