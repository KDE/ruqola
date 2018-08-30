/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "resetavatarjobtest.h"
#include "users/resetavatarjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ResetAvatarJobTest)

ResetAvatarJobTest::ResetAvatarJobTest(QObject *parent)
    : QObject(parent)
{
}

void ResetAvatarJobTest::shouldHaveDefaultValue()
{
    ResetAvatarJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.avatarUserId().isEmpty());
}

void ResetAvatarJobTest::shouldGenerateRequest()
{
    ResetAvatarJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.resetAvatar")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ResetAvatarJobTest::shouldGenerateJson()
{
    ResetAvatarJob job;
    const QString avatarid = QStringLiteral("foo1");
    job.setAvatarUserId(avatarid);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"userId\":\"%1\"}").arg(avatarid).toLatin1());
}

void ResetAvatarJobTest::shouldNotStarting()
{
    ResetAvatarJob job;

    RestApiMethod *method = new RestApiMethod;
    method->setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(method);

    QNetworkAccessManager *mNetworkAccessManager = new QNetworkAccessManager;
    job.setNetworkAccessManager(mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString avatarid = QStringLiteral("foo1");
    job.setAvatarUserId(avatarid);
    QVERIFY(job.canStart());

    delete method;
    delete mNetworkAccessManager;
}
