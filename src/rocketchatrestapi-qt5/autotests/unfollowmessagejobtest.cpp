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

#include "unfollowmessagejobtest.h"
#include "chat/unfollowmessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(UnFollowMessageJobTest)
using namespace RocketChatRestApi;
UnFollowMessageJobTest::UnFollowMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void UnFollowMessageJobTest::shouldHaveDefaultValue()
{
    UnFollowMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UnFollowMessageJobTest::shouldGenerateRequest()
{
    UnFollowMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.unfollowMessage")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UnFollowMessageJobTest::shouldGenerateJson()
{
    UnFollowMessageJob job;
    const QString messageid = QStringLiteral("foo1");
    job.setMessageId(messageid);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"mid\":\"%1\"}").arg(messageid).toLatin1());
}

void UnFollowMessageJobTest::shouldNotStarting()
{
    UnFollowMessageJob job;

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
    const QString messageId = QStringLiteral("foo1");
    job.setMessageId(messageId);
    QVERIFY(job.canStart());
}
