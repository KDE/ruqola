/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleslistjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/roleslistjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(RolesListJobTest)
using namespace RocketChatRestApi;
RolesListJobTest::RolesListJobTest(QObject *parent)
    : QObject(parent)
{
}

void RolesListJobTest::shouldHaveDefaultValue()
{
    RolesListJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RolesListJobTest::shouldGenerateRequest()
{
    RolesListJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/roles.list"_s));
    }
}

#include "moc_roleslistjobtest.cpp"
