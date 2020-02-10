/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "usersautocompletejobtest.h"
#include "users/usersautocompletejob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
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
