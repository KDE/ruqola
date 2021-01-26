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

#include "customuserstatusupdatetestjob.h"
#include "custom/customuserstatusupdatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(CustomUserStatusUpdateTestJob)
using namespace RocketChatRestApi;
CustomUserStatusUpdateTestJob::CustomUserStatusUpdateTestJob(QObject *parent)
    : QObject(parent)
{
}

void CustomUserStatusUpdateTestJob::shouldHaveDefaultValue()
{
    CustomUserStatusUpdateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CustomUserStatusUpdateTestJob::shouldGenerateRequest()
{
    CustomUserStatusUpdateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/custom-user-status.update")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void CustomUserStatusUpdateTestJob::shouldGenerateJson()
{
    CustomUserStatusUpdateJob job;

    const QString name = QStringLiteral("foo1");
    const QString statusType = QStringLiteral("topic1");
    const QString identifier = QStringLiteral("id1");
    CustomUserStatusUpdateJob::StatusUpdateInfo info;
    info.name = name;
    info.statusType = statusType;
    info.identifier = identifier;
    job.setStatusUpdateInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"_id":"%3","name":"%1","statusType":"%2"})").arg(name, statusType, identifier).toLatin1());
}

void CustomUserStatusUpdateTestJob::shouldNotStarting()
{
    CustomUserStatusUpdateJob job;

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

    const QString name = QStringLiteral("foo1");
    const QString statusType = QStringLiteral("topic1");
    const QString identifier = QStringLiteral("id1");

    CustomUserStatusUpdateJob::StatusUpdateInfo info;
    info.name = name;
    job.setStatusUpdateInfo(info);
    QVERIFY(!job.canStart());
    info.statusType = statusType;
    job.setStatusUpdateInfo(info);
    QVERIFY(!job.canStart());
    info.identifier = identifier;
    job.setStatusUpdateInfo(info);
    QVERIFY(job.canStart());
}
