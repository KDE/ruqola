/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listoauthappsjobtest.h"
#include "misc/listoauthappsjob.h"
#include "ruqola_restapi_helper.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(ListOauthAppsJobTest)
using namespace RocketChatRestApi;
ListOauthAppsJobTest::ListOauthAppsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ListOauthAppsJobTest::shouldHaveDefaultValue()
{
    ListOauthAppsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void ListOauthAppsJobTest::shouldGenerateRequest()
{
    ListOauthAppsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/oauth-apps.list")));
}

#include "moc_listoauthappsjobtest.cpp"
