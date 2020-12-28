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

#include "userssetpreferencesjobtest.h"
#include "users/userssetpreferencesjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(UsersSetPreferencesJobTest)
using namespace RocketChatRestApi;
UsersSetPreferencesJobTest::UsersSetPreferencesJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersSetPreferencesJobTest::shouldHaveDefaultValue()
{
    UsersSetPreferencesJob job;
    QVERIFY(!job.usersSetPreferencesInfo().isValid());
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UsersSetPreferencesJobTest::shouldGenerateRequest()
{
    UsersSetPreferencesJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.setPreferences")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UsersSetPreferencesJobTest::shouldGenerateJson()
{
    UsersSetPreferencesJob job;
    UsersSetPreferencesJob::UsersSetPreferencesInfo info;
    const QString desktopNotifications = QStringLiteral("Bla");
    info.desktopNotifications = desktopNotifications;
    job.setUsersSetPreferencesInfo(info);
    QVERIFY(!job.canStart());

    const QString userId = QStringLiteral("foo");
    info.userId = userId;
    job.setUsersSetPreferencesInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"data":{"desktopNotifications":"%2","highlights":[]},"userId":"%1"})").arg(userId, desktopNotifications).toLatin1());
}

void UsersSetPreferencesJobTest::shouldNotStarting()
{
    UsersSetPreferencesJob job;

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
    UsersSetPreferencesJob::UsersSetPreferencesInfo info;
    info.desktopNotifications = QStringLiteral("Bla");
    job.setUsersSetPreferencesInfo(info);
    QVERIFY(!job.canStart());

    info.userId = QStringLiteral("foo");
    job.setUsersSetPreferencesInfo(info);
    QVERIFY(job.canStart());
}
