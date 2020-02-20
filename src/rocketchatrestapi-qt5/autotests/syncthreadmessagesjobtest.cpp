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

#include "syncthreadmessagesjobtest.h"
#include "chat/syncthreadmessagesjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(SyncThreadMessagesJobTest)
using namespace RocketChatRestApi;
SyncThreadMessagesJobTest::SyncThreadMessagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void SyncThreadMessagesJobTest::shouldHaveDefaultValue()
{
    SyncThreadMessagesJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.threadMessageId().isEmpty());
    QVERIFY(job.timeStamp().isEmpty());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SyncThreadMessagesJobTest::shouldGenerateRequest()
{
    SyncThreadMessagesJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString threadMessageId = QStringLiteral("bla");
    job.setThreadMessageId(threadMessageId);

    const QString timestamp = QStringLiteral("blu");
    job.setTimeStamp(timestamp);

    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.syncThreadMessages?tmid=%1&updatedSince=%2").arg(threadMessageId, timestamp)));
}

void SyncThreadMessagesJobTest::shouldNotStarting()
{
    SyncThreadMessagesJob job;

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
    const QString threadMessageId = QStringLiteral("foo1");
    job.setThreadMessageId(threadMessageId);
    QVERIFY(!job.canStart());

    const QString timestamp = QStringLiteral("blu");
    job.setTimeStamp(timestamp);
    QVERIFY(job.canStart());
}
