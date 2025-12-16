/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersautocompletejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include "users/usersautocompletejob.h"
#include <QTest>

QTEST_GUILESS_MAIN(UsersAutocompleteJobTest)
using namespace RocketChatRestApi;
UsersAutocompleteJobTest::UsersAutocompleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersAutocompleteJobTest::shouldHaveDefaultValue()
{
    UsersAutocompleteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
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
    info.pattern = u"foo"_s;
    job.setUsersCompleterInfo(info);
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url().toString(), u"http://www.kde.org/api/v1/users.autocomplete?selector=%7B%22term%22: %22foo%22%7D"_s);
}

#include "moc_usersautocompletejobtest.cpp"
