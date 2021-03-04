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

#include "usersupdatejobtest.h"
#include "ruqola_restapi_helper.h"
#include "users/usersupdatejob.h"
#include <QTest>
QTEST_GUILESS_MAIN(UsersUpdateJobTest)
using namespace RocketChatRestApi;
UsersUpdateJobTest::UsersUpdateJobTest(QObject *parent)
    : QObject(parent)
{
}

void UsersUpdateJobTest::shouldHaveDefaultValue()
{
    UsersUpdateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UsersUpdateJobTest::shouldGenerateRequest()
{
    UsersUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.update")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UsersUpdateJobTest::shouldGenerateJson()
{
    UsersUpdateJob job;
    UsersUpdateJob::UpdateInfo info;
    //    const QString desktopNotifications = QStringLiteral("Bla");
    //    info.desktopNotifications = desktopNotifications;
    //    job.setUsersSetPreferencesInfo(info);
    //    QVERIFY(!job.canStart());

    //    const QString userId = QStringLiteral("foo");
    //    info.userId = userId;
    //    job.setUsersSetPreferencesInfo(info);
    //    QCOMPARE(
    //        job.json().toJson(QJsonDocument::Compact),
    //        QStringLiteral(
    //            R"({"data":{"convertAsciiEmoji":true,"desktopNotifications":"%2","hideAvatars":false,"hideRoles":false,"highlights":[],"useEmojis":true},"userId":"%1"})")
    //            .arg(userId, desktopNotifications)
    //            .toLatin1());

    // TODO
}

void UsersUpdateJobTest::shouldNotStarting()
{
    UsersUpdateJob job;

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

    UsersUpdateJob::UpdateInfo info;
    info.mUserId = QStringLiteral("userid");
    job.setUpdateInfo(info);

    QVERIFY(job.canStart());
}
