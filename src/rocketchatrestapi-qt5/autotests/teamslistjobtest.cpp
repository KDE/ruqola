/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "teamslistjobtest.h"
#include "ruqola_restapi_helper.h"
#include "teams/teamslistjob.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(TeamsListJobTest)
using namespace RocketChatRestApi;
TeamsListJobTest::TeamsListJobTest(QObject *parent)
    : QObject(parent)
{
}

void TeamsListJobTest::shouldHaveDefaultValue()
{
    TeamsListJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(job.pattern().isEmpty());
}

void TeamsListJobTest::shouldGenerateRequest()
{
    TeamsListJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.list?userId=foo1")));
}

void TeamsListJobTest::shouldGenerateRequestUsername()
{
    TeamsListJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.list?username=foo1")));
}
