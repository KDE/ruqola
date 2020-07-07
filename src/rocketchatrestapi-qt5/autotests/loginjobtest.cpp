/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "loginjobtest.h"
#include "authentication/loginjob.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(LoginJobTest)

using namespace RocketChatRestApi;
LoginJobTest::LoginJobTest(QObject *parent)
    : QObject(parent)
{
}

void LoginJobTest::shouldHaveDefaultValue()
{
    LoginJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(job.authToken().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}


void LoginJobTest::shouldHaveArguments()
{
    LoginJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    QVERIFY(!job.canStart());
    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    job.setPassword(QStringLiteral("bla"));
    QVERIFY(!job.canStart());
    job.setUserName(QStringLiteral("foo"));
    QVERIFY(job.canStart());
}

void LoginJobTest::shouldGenerateLoginRequest()
{
    LoginJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/login")));
}


void LoginJobTest::shouldGenerateJson()
{
    LoginJob job;
    const QString password(QStringLiteral("bla"));
    const QString username(QStringLiteral("foo"));
    job.setPassword(QStringLiteral("bla"));
    job.setUserName(QStringLiteral("foo"));
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"password\":\"%1\",\"user\":\"%2\"}").arg(password).arg(username).toLatin1());
}
