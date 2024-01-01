/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersautocompletejobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/usersautocompletejob.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(UsersAutocompleteJobTest)
using namespace RocketChatRestApi;
UsersAutocompleteJobTest::UsersAutocompleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersAutocompleteJobTest::shouldHaveDefaultValue()
{
    UsersAutocompleteJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());

    UsersAutocompleteJob::UsersAutocompleterInfo info;
    QVERIFY(!info.isValid());
    QVERIFY(info.pattern.isEmpty());
    QVERIFY(info.exception.isEmpty());
}

void UsersAutocompleteJobTest::shouldGenerateRequest()
{
    UsersAutocompleteJob job;
    UsersAutocompleteJob::UsersAutocompleterInfo info;
    info.pattern = QStringLiteral("foo");
    job.setUsersCompleterInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url().toString(), QStringLiteral("http://www.kde.org/api/v1/users.autocomplete?selector=%7B%22term%22: %22foo%22%7D"));
}

#include "moc_usersautocompletejobtest.cpp"
