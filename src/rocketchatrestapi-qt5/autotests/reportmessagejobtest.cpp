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

#include "reportmessagejobtest.h"
#include "ruqola_restapi_helper.h"
#include "chat/reportmessagejob.h"
#include <QTest>
#include <QJsonDocument>
#include <restapimethod.h>

QTEST_GUILESS_MAIN(ReportMessageJobTest)
using namespace RocketChatRestApi;
ReportMessageJobTest::ReportMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void ReportMessageJobTest::shouldHaveDefaultValue()
{
    ReportMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(job.reportMessage().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ReportMessageJobTest::shouldHaveMessageId()
{
    ReportMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
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
    job.setReportMessage(QStringLiteral("bla"));
    QVERIFY(!job.canStart());
    job.setMessageId(QStringLiteral("bla"));
    QVERIFY(job.canStart());
}

void ReportMessageJobTest::shouldGenerateJobRequest()
{
    ReportMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.reportMessage")));
}

void ReportMessageJobTest::shouldGenerateJson()
{
    ReportMessageJob job;
    const QString messageId = QStringLiteral("foo1");
    job.setMessageId(messageId);
    const QString reportMessage = QStringLiteral("foo2");
    job.setReportMessage(reportMessage);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"description\":\"%2\",\"messageId\":\"%1\"}").arg(messageId, reportMessage).toLatin1());
}
