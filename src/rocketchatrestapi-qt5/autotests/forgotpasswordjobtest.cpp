/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "forgotpasswordjobtest.h"
#include "users/forgotpasswordjob.h"
#include "restapimethod.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ForgotPasswordJobTest)
using namespace RocketChatRestApi;
ForgotPasswordJobTest::ForgotPasswordJobTest(QObject *parent)
    : QObject(parent)
{
}

void ForgotPasswordJobTest::shouldHaveDefaultValue()
{
    ForgotPasswordJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(job.email().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ForgotPasswordJobTest::shouldGenerateRequest()
{
    ForgotPasswordJob job;
    RestApiMethod *method = new RestApiMethod;
    method->setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(method);
    job.setEmail(QStringLiteral("foo"));
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.forgotPassword")));
    delete method;
}

void ForgotPasswordJobTest::shouldGenerateJson()
{
    ForgotPasswordJob job;
    const QString email = QStringLiteral("foo");
    job.setEmail(email);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"email\":\"%1\"}").arg(email).toLatin1());
}
