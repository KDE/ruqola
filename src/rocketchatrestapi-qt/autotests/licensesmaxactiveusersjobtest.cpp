/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesmaxactiveusersjobtest.h"
#include "license/licensesisenterprisejob.h"
#include "ruqola_restapi_helper.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(LicensesMaxActiveUsersJobTest)
using namespace RocketChatRestApi;
LicensesMaxActiveUsersJobTest::LicensesMaxActiveUsersJobTest(QObject *parent)
    : QObject(parent)
{
}

void LicensesMaxActiveUsersJobTest::shouldHaveDefaultValue()
{
    LicensesIsEnterpriseJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void LicensesMaxActiveUsersJobTest::shouldGenerateRequest()
{
    LicensesIsEnterpriseJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/licenses.isEnterprise")));
    }
}

#include "moc_licensesmaxactiveusersjobtest.cpp"
