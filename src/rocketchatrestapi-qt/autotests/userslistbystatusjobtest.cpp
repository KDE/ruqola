/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userslistbystatusjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include "users/userslistbystatusjob.h"
#include <QTest>

QTEST_GUILESS_MAIN(UsersListByStatusJobTest)
using namespace RocketChatRestApi;
UsersListByStatusJobTest::UsersListByStatusJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersListByStatusJobTest::shouldHaveDefaultValue()
{
    UsersListByStatusJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
}

void UsersListByStatusJobTest::shouldGenerateRequest()
{
    UsersListByStatusJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.listByStatus"_s));

    {
        UsersListByStatusJob::UsersListByStatusJobInfo info;
        info.status = RocketChatRestApi::UsersListByStatusJob::Status::Activated;
        info.type = RocketChatRestApi::UsersListByStatusJob::StatusType::User;
        job.setUsersListByStatusJobInfo(info);
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.listByStatus?status=active&type=user"_s));
    }
    {
        UsersListByStatusJob::UsersListByStatusJobInfo info;
        info.status = RocketChatRestApi::UsersListByStatusJob::Status::Deactivated;
        info.type = RocketChatRestApi::UsersListByStatusJob::StatusType::User;
        info.hasLoggedIn = RocketChatRestApi::UsersListByStatusJob::LoggedStatus::Logged;
        job.setUsersListByStatusJobInfo(info);
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.listByStatus?hasLoggedIn=true&status=deactivated&type=user"_s));
    }
    {
        UsersListByStatusJob::UsersListByStatusJobInfo info;
        info.status = RocketChatRestApi::UsersListByStatusJob::Status::Deactivated;
        info.type = RocketChatRestApi::UsersListByStatusJob::StatusType::User;
        info.hasLoggedIn = RocketChatRestApi::UsersListByStatusJob::LoggedStatus::Logged;
        info.roles = {"user"_L1};
        job.setUsersListByStatusJobInfo(info);
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.listByStatus?hasLoggedIn=true&status=deactivated&type=user&roles[]=user"_s));
    }
    {
        UsersListByStatusJob::UsersListByStatusJobInfo info;
        info.status = RocketChatRestApi::UsersListByStatusJob::Status::Deactivated;
        info.type = RocketChatRestApi::UsersListByStatusJob::StatusType::User;
        info.hasLoggedIn = RocketChatRestApi::UsersListByStatusJob::LoggedStatus::Logged;
        info.roles = {"user"_L1, "admin"_L1};
        job.setUsersListByStatusJobInfo(info);
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(),
                 QUrl(u"http://www.kde.org/api/v1/users.listByStatus?hasLoggedIn=true&status=deactivated&type=user&roles[]=user&roles[]=admin"_s));
    }

    {
        UsersListByStatusJob::UsersListByStatusJobInfo info;
        info.status = RocketChatRestApi::UsersListByStatusJob::Status::Deactivated;
        info.type = RocketChatRestApi::UsersListByStatusJob::StatusType::User;
        info.hasLoggedIn = RocketChatRestApi::UsersListByStatusJob::LoggedStatus::Logged;
        info.roles = {"user"_L1, "admin"_L1};
        info.searchName = "gla"_L1;
        job.setUsersListByStatusJobInfo(info);
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(),
                 QUrl(QStringLiteral(
                     "http://www.kde.org/api/v1/users.listByStatus?hasLoggedIn=true&status=deactivated&type=user&roles[]=user&roles[]=admin&searchTerm=gla")));
    }
}

void UsersListByStatusJobTest::shouldUsersListByStatusJobInfoValue()
{
    const UsersListByStatusJob::UsersListByStatusJobInfo info;
    QCOMPARE(info.status, RocketChatRestApi::UsersListByStatusJob::Status::Unknown);
    QCOMPARE(info.type, RocketChatRestApi::UsersListByStatusJob::StatusType::Unknown);
    QCOMPARE(info.hasLoggedIn, RocketChatRestApi::UsersListByStatusJob::LoggedStatus::Unknown);
    QVERIFY(info.roles.isEmpty());
    QVERIFY(info.searchName.isEmpty());
    QVERIFY(info.isValid());
}

#include "moc_userslistbystatusjobtest.cpp"
