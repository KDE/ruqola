/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsautocompleteavailableforteamsjobtest.h"
#include "restapimethod.h"
#include "teams/roomsautocompleteavailableforteamsjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoomsAutocompleteAvailableForTeamsJobTest)
using namespace RocketChatRestApi;
RoomsAutocompleteAvailableForTeamsJobTest::RoomsAutocompleteAvailableForTeamsJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsAutocompleteAvailableForTeamsJobTest::shouldHaveDefaultValue()
{
    RoomsAutocompleteAvailableForTeamsJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.roomsCompleterInfo().isValid());
}

void RoomsAutocompleteAvailableForTeamsJobTest::shouldGenerateRequest()
{
    RoomsAutocompleteAvailableForTeamsJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    RoomsAutocompleteAvailableForTeamsJob::RoomsAutocompleteChannelAndPrivateInfo info;
    info.name = QStringLiteral("foo");
    job.setRoomsCompleterInfo(info);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url().toString(), QStringLiteral("http://www.kde.org/api/v1/rooms.autocomplete.availableForTeams?name=foo"));
}

#include "moc_roomsautocompleteavailableforteamsjobtest.cpp"
