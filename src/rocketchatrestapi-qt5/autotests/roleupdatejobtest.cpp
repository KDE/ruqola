/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "roleupdatejobtest.h"
#include "role/rolecreatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(RoleUpdateJobTest)
using namespace RocketChatRestApi;
RoleUpdateJobTest::RoleUpdateJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoleUpdateJobTest::shouldHaveDefaultValue()
{
    RoleCreateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RoleUpdateJobTest::shouldGenerateRequest()
{
    RoleCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/role.create")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RoleUpdateJobTest::shouldGenerateJson()
{
    RoleCreateJob job;

    const QString name = QStringLiteral("foo1");
    const QString statusType = QStringLiteral("topic1");
    RoleCreateJob::RoleCreateInfo info;
    info.name = name;
    info.description = statusType;
    job.setCreateRoleInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"name":"%1","statusType":"%2"})").arg(name, statusType).toLatin1());
}

void RoleUpdateJobTest::shouldNotStarting()
{
    RoleCreateJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);

    RoleCreateJob::RoleCreateInfo info;
    info.name = QStringLiteral("foo");
    job.setCreateRoleInfo(info);
    QVERIFY(!job.canStart());
    info.description = QStringLiteral("bla");
    job.setCreateRoleInfo(info);
    QVERIFY(job.canStart());
}
