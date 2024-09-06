/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userslistbystatusjobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/userslistbystatusjob.h"
QTEST_GUILESS_MAIN(UsersListByStatusJobTest)
using namespace RocketChatRestApi;
UsersListByStatusJobTest::UsersListByStatusJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersListByStatusJobTest::shouldHaveDefaultValue()
{
    UsersListByStatusJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
}

void UsersListByStatusJobTest::shouldGenerateRequest()
{
    UsersListByStatusJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.listByStatus?hasLoggedIn=false")));

    {
        UsersListByStatusJob::UsersListByStatusJobInfo info;
        info.status = RocketChatRestApi::UsersListByStatusJob::Status::Activated;
        info.type = RocketChatRestApi::UsersListByStatusJob::StatusType::User;
        job.setUsersListByStatusJobInfo(info);
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.listByStatus?hasLoggedIn=false&status=active&type=user")));
    }
    {
        UsersListByStatusJob::UsersListByStatusJobInfo info;
        info.status = RocketChatRestApi::UsersListByStatusJob::Status::Desactivated;
        info.type = RocketChatRestApi::UsersListByStatusJob::StatusType::User;
        info.hasLoggedIn = true;
        job.setUsersListByStatusJobInfo(info);
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.listByStatus?hasLoggedIn=true&status=deactivated&type=user")));
    }
    {
        UsersListByStatusJob::UsersListByStatusJobInfo info;
        info.status = RocketChatRestApi::UsersListByStatusJob::Status::Desactivated;
        info.type = RocketChatRestApi::UsersListByStatusJob::StatusType::User;
        info.hasLoggedIn = true;
        info.roles = {"user"_L1};
        job.setUsersListByStatusJobInfo(info);
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(),
                 QUrl(QStringLiteral("http://www.kde.org/api/v1/users.listByStatus?hasLoggedIn=true&status=deactivated&type=user&roles[]=user")));
    }
    {
        UsersListByStatusJob::UsersListByStatusJobInfo info;
        info.status = RocketChatRestApi::UsersListByStatusJob::Status::Desactivated;
        info.type = RocketChatRestApi::UsersListByStatusJob::StatusType::User;
        info.hasLoggedIn = true;
        info.roles = {"user"_L1, "admin"_L1};
        job.setUsersListByStatusJobInfo(info);
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(),
                 QUrl(QStringLiteral("http://www.kde.org/api/v1/users.listByStatus?hasLoggedIn=true&status=deactivated&type=user&roles[]=user&roles[]=admin")));
    }
}

void UsersListByStatusJobTest::shouldUsersListByStatusJobInfoValue()
{
    UsersListByStatusJob::UsersListByStatusJobInfo info;
    QCOMPARE(info.status, RocketChatRestApi::UsersListByStatusJob::Status::Unknown);
    QCOMPARE(info.type, RocketChatRestApi::UsersListByStatusJob::StatusType::Unknown);
    QVERIFY(!info.hasLoggedIn);
    QVERIFY(info.isValid());
}

#include "moc_userslistbystatusjobtest.cpp"
