/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionslistalljobtest.h"
#include "permissions/permissionslistalljob.h"
#include "ruqola_restapi_helper.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(PermissionsListAllJobTest)
using namespace RocketChatRestApi;
PermissionsListAllJobTest::PermissionsListAllJobTest(QObject *parent)
    : QObject(parent)
{
}

void PermissionsListAllJobTest::shouldHaveDefaultValue()
{
    PermissionsListAllJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void PermissionsListAllJobTest::shouldGenerateRequest()
{
    PermissionsListAllJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/permissions.listAll")));
}

#include "moc_permissionslistalljobtest.cpp"
