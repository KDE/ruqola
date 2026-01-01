/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userspresencejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include "users/userspresencejob.h"
#include <QTest>

QTEST_GUILESS_MAIN(UsersPresenceJobTest)
using namespace RocketChatRestApi;
UsersPresenceJobTest::UsersPresenceJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersPresenceJobTest::shouldHaveDefaultValue()
{
    UsersPresenceJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UsersPresenceJobTest::shouldGenerateRequest()
{
    UsersPresenceJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.presence"_s));
}

#include "moc_userspresencejobtest.cpp"
