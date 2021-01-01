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

#include "user2fasendemailcodejobtest.h"
#include "2fa/user2fasendemailcodejob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(User2FASendEmailCodeJobTest)
using namespace RocketChatRestApi;
User2FASendEmailCodeJobTest::User2FASendEmailCodeJobTest(QObject *parent)
    : QObject(parent)
{
}

void User2FASendEmailCodeJobTest::shouldHaveDefaultValue()
{
    User2FASendEmailCodeJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void User2FASendEmailCodeJobTest::shouldGenerateRequest()
{
    User2FASendEmailCodeJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.2fa.sendEmailCode")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void User2FASendEmailCodeJobTest::shouldGenerateJson()
{
    User2FASendEmailCodeJob job;
    const QString emails = QStringLiteral("emails");
    job.setUsernameOrEmail(emails);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"emailOrUsername":"%1"})").arg(emails).toLatin1());
}

void User2FASendEmailCodeJobTest::shouldNotStarting()
{
    User2FASendEmailCodeJob job;

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
    job.setUsernameOrEmail(QStringLiteral("emails"));
    QVERIFY(job.canStart());
}
