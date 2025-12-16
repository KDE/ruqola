/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionslistalljobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "permissions/permissionslistalljob.h"

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

QTEST_GUILESS_MAIN(PermissionsListAllJobTest)
using namespace RocketChatRestApi;
PermissionsListAllJobTest::PermissionsListAllJobTest(QObject *parent)
    : QObject(parent)
{
}

void PermissionsListAllJobTest::shouldHaveDefaultValue()
{
    PermissionsListAllJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void PermissionsListAllJobTest::shouldGenerateRequest()
{
    PermissionsListAllJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/permissions.listAll"_s));
}

#include "moc_permissionslistalljobtest.cpp"
