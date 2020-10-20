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

#include "customuserstatuscreatetestjob.h"
#include "custom/customuserstatuscreatejob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(CustomUserStatusCreateTestJob)
using namespace RocketChatRestApi;
CustomUserStatusCreateTestJob::CustomUserStatusCreateTestJob(QObject *parent)
    : QObject(parent)
{
}

void CustomUserStatusCreateTestJob::shouldHaveDefaultValue()
{
    CustomUserStatusCreateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CustomUserStatusCreateTestJob::shouldGenerateRequest()
{
    CustomUserStatusCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/custom-user-status.create")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void CustomUserStatusCreateTestJob::shouldGenerateJson()
{
    CustomUserStatusCreateJob job;

    const QString name = QStringLiteral("foo1");
    const QString statusType = QStringLiteral("topic1");
    CustomUserStatusCreateJob::StatusCreateInfo info;
    info.name = name;
    info.statusType = statusType;
    job.setStatusCreateInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"name\":\"%1\",\"statusType\":\"%2\"}").arg(name, statusType).toLatin1());
}

void CustomUserStatusCreateTestJob::shouldNotStarting()
{
    CustomUserStatusCreateJob job;

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

    CustomUserStatusCreateJob::StatusCreateInfo info;
    info.name = QStringLiteral("foo");
    job.setStatusCreateInfo(info);
    QVERIFY(!job.canStart());
    info.statusType = QStringLiteral("bla");
    job.setStatusCreateInfo(info);
    QVERIFY(job.canStart());
}
