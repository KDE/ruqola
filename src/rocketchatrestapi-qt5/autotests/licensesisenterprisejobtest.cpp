/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesisenterprisejobtest.h"
#include "license/licensesisenterprisejob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(LicensesIsEnterpriseJobTest)
using namespace RocketChatRestApi;
LicensesIsEnterpriseJobTest::LicensesIsEnterpriseJobTest(QObject *parent)
    : QObject(parent)
{
}

void LicensesIsEnterpriseJobTest::shouldHaveDefaultValue()
{
    LicensesIsEnterpriseJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void LicensesIsEnterpriseJobTest::shouldGenerateRequest()
{
    LicensesIsEnterpriseJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/licenses.isEnterprise")));
    }
}
