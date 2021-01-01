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

#include "deleteownaccountjobtest.h"
#include "users/deleteownaccountjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(DeleteOwnAccountJobTest)
using namespace RocketChatRestApi;
DeleteOwnAccountJobTest::DeleteOwnAccountJobTest(QObject *parent)
    : QObject(parent)
{
}

void DeleteOwnAccountJobTest::shouldHaveDefaultValue()
{
    DeleteOwnAccountJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.password().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void DeleteOwnAccountJobTest::shouldGenerateRequest()
{
    DeleteOwnAccountJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.deleteOwnAccount")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void DeleteOwnAccountJobTest::shouldGenerateJson()
{
    DeleteOwnAccountJob job;
    const QString password = QStringLiteral("foo1");
    job.setPassword(password);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"password":"bb4eca334f61af3b67b5d528907d30285151610200539302f4c8cabe66225b53"})").toLatin1());
}

void DeleteOwnAccountJobTest::shouldNotStarting()
{
    DeleteOwnAccountJob job;

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
    QVERIFY(!job.canStart());
    const QString password = QStringLiteral("foo1");
    job.setPassword(password);
    QVERIFY(job.canStart());
}
