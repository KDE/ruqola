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

#include "customuserstatusdeletetestjob.h"
#include "custom/customuserstatusdeletejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(CustomUserStatusDeleteTestJob)
using namespace RocketChatRestApi;
CustomUserStatusDeleteTestJob::CustomUserStatusDeleteTestJob(QObject *parent)
    : QObject(parent)
{
}

void CustomUserStatusDeleteTestJob::shouldHaveDefaultValue()
{
    CustomUserStatusDeleteJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CustomUserStatusDeleteTestJob::shouldGenerateRequest()
{
    CustomUserStatusDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/custom-user-status.delete")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void CustomUserStatusDeleteTestJob::shouldGenerateJson()
{
    CustomUserStatusDeleteJob job;

    const QString statusId = QStringLiteral("foo1");
    job.setCustomUserStatusId(statusId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"customUserStatusId":"%1"})").arg(statusId).toLatin1());
}

void CustomUserStatusDeleteTestJob::shouldNotStarting()
{
    CustomUserStatusDeleteJob job;

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

    const QString statusId = QStringLiteral("foo1");
    job.setCustomUserStatusId(statusId);
    QVERIFY(job.canStart());
}
