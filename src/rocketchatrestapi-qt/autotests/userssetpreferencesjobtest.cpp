/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userssetpreferencesjobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/userssetpreferencesjob.h"
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
        QCOMPARE(job.json().toJson(QJsonDocument::Compact),
                 QStringLiteral(R"({"data":{"desktopNotifications":"%2"},"userId":"%1"})").arg(userId, desktopNotifications).toLatin1());
    }
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

#include "moc_userssetpreferencesjobtest.cpp"
