/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappsjobtest.h"
#include "misc/listoauthappsjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(OauthAppsJobTest)
using namespace RocketChatRestApi;
OauthAppsJobTest::OauthAppsJobTest(QObject *parent)
    : QObject(parent)
{
}

void OauthAppsJobTest::shouldHaveDefaultValue()
{
    ListOauthAppsJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void OauthAppsJobTest::shouldGenerateRequest()
{
    ListOauthAppsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/oauth-apps.list")));
}
