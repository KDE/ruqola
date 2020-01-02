/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "setstatusjobtest.h"
#include "users/setstatusjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(SetStatusJobTest)
using namespace RocketChatRestApi;
SetStatusJobTest::SetStatusJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetStatusJobTest::shouldHaveDefaultValue()
{
    SetStatusJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.statusUserId().isEmpty());
    QVERIFY(job.statusMessage().isEmpty());
    QCOMPARE(job.status(), SetStatusJob::Unknown);
    QVERIFY(!job.hasQueryParameterSupport());
}

void SetStatusJobTest::shouldGenerateRequest()
{
    SetStatusJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.setStatus")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void SetStatusJobTest::shouldGenerateJson()
{
    SetStatusJob job;
    const QString userId = QStringLiteral("foo1");
    job.setStatusUserId(userId);
    job.setStatusMessage(QString());
    job.setStatus(SetStatusJob::Away);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"message\":\"\",\"status\":\"away\",\"userId\":\"foo1\"}").arg(userId).toLatin1());
    job.setStatusMessage(QStringLiteral("bla"));
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"message\":\"bla\",\"status\":\"away\",\"userId\":\"foo1\"}").arg(userId).toLatin1());
    job.setStatus(SetStatusJob::Offline);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"message\":\"bla\",\"status\":\"offline\",\"userId\":\"foo1\"}").arg(userId).toLatin1());
}

void SetStatusJobTest::shouldNotStarting()
{
    SetStatusJob job;

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
    const QString statusUserid = QStringLiteral("foo1");
    job.setStatusUserId(statusUserid);
    QVERIFY(!job.canStart());
    job.setStatus(SetStatusJob::Away);
    QVERIFY(job.canStart());

    delete method;
    delete mNetworkAccessManager;
}
