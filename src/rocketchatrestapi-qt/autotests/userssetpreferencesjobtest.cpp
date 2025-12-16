/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userssetpreferencesjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "users/userssetpreferencesjob.h"
#include <QJsonDocument>
#include <QTest>
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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UsersSetPreferencesJobTest::shouldGenerateRequest()
{
    UsersSetPreferencesJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.setPreferences"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void UsersSetPreferencesJobTest::shouldGenerateJson()
{
    {
        UsersSetPreferencesJob job;
        UsersSetPreferencesJob::UsersSetPreferencesInfo info;
        const QString desktopNotifications = u"Bla"_s;
        info.desktopNotifications = desktopNotifications;
        job.setUsersSetPreferencesInfo(info);
        QVERIFY(!job.canStart());

        const QByteArray userId = "foo"_ba;
        info.userId = userId;
        job.setUsersSetPreferencesInfo(info);
        QCOMPARE(job.json().toJson(QJsonDocument::Compact),
                 QStringLiteral(R"({"data":{"desktopNotifications":"%2"},"userId":"%1"})").arg(QLatin1StringView(userId), desktopNotifications).toLatin1());
    }
}

void UsersSetPreferencesJobTest::shouldNotStarting()
{
    UsersSetPreferencesJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    UsersSetPreferencesJob::UsersSetPreferencesInfo info;
    info.desktopNotifications = u"Bla"_s;
    job.setUsersSetPreferencesInfo(info);
    QVERIFY(!job.canStart());

    info.userId = "foo"_ba;
    job.setUsersSetPreferencesInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_userssetpreferencesjobtest.cpp"
