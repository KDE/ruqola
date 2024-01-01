/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listinvitejobtest.h"
#include "invite/listinvitejob.h"
#include "ruqola_restapi_helper.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(ListInviteJobTest)
using namespace RocketChatRestApi;
ListInviteJobTest::ListInviteJobTest(QObject *parent)
    : QObject(parent)
{
}

void ListInviteJobTest::shouldHaveDefaultValue()
{
    ListInviteJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void ListInviteJobTest::shouldGenerateRequest()
{
    ListInviteJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/listInvites")));
    }
}

void ListInviteJobTest::shouldNotStarting()
{
    ListInviteJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager networkAccessManager;
    job.setNetworkAccessManager(&networkAccessManager);
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    job.setUserId(userId);
    QVERIFY(job.canStart());
}

#include "moc_listinvitejobtest.cpp"
