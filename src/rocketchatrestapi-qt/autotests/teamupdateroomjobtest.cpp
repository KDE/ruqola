/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamupdateroomjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"

#include "teams/teamupdateroomjob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(TeamUpdateRoomJobTest)
using namespace RocketChatRestApi;
TeamUpdateRoomJobTest::TeamUpdateRoomJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamUpdateRoomJobTest::shouldHaveDefaultValue()
{
    TeamUpdateRoomJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.isDefault());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void TeamUpdateRoomJobTest::shouldGenerateRequest()
{
    TeamUpdateRoomJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/teams.updateRoom"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void TeamUpdateRoomJobTest::shouldGenerateJson()
{
    TeamUpdateRoomJob job;
    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    const bool isDefault = true;
    job.setIsDefault(isDefault);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"isDefault":true,"roomId":"%1"})"_s.arg(QLatin1StringView(roomId)).toLatin1());
}

void TeamUpdateRoomJobTest::shouldNotStarting()
{
    TeamUpdateRoomJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_teamupdateroomjobtest.cpp"
