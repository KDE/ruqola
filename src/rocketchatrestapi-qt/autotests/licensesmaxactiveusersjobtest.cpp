/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesmaxactiveusersjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "license/licensesisenterprisejob.h"

#include "ruqola_restapi_helper.h"
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
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/licenses.isEnterprise"_s));
    }
}

#include "moc_licensesmaxactiveusersjobtest.cpp"
