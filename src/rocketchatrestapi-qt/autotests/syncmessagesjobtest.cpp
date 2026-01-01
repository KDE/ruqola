/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "syncmessagesjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/syncmessagesjob.h"
#include "restapimethod.h"
#include <QTest>

QTEST_GUILESS_MAIN(SyncMessagesJobTest)
using namespace RocketChatRestApi;
SyncMessagesJobTest::SyncMessagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void SyncMessagesJobTest::shouldHaveDefaultValue()
{
    SyncMessagesJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void SyncMessagesJobTest::shouldGenerateRequest()
{
    SyncMessagesJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QByteArray roomId = "bla"_ba;
    job.setRoomId(roomId);
    const QDateTime lastUpdate{QDateTime(QDate(2023, 7, 21), QTime(1, 1, 1))};
    job.setLastUpdate(lastUpdate);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(),
             QUrl(u"http://www.kde.org/api/v1/chat.syncMessages?roomId=%1&lastUpdate=%2"_s.arg(QLatin1StringView(roomId))
                      .arg(lastUpdate.toUTC().toString(Qt::ISODateWithMs))));
}

void SyncMessagesJobTest::shouldNotStarting()
{
    SyncMessagesJob job;

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
    const QByteArray roomId = "foo1"_ba;
    job.setRoomId(roomId);
    QVERIFY(!job.canStart());
    const QDateTime lastUpdate{QDateTime::currentDateTime()};
    job.setLastUpdate(lastUpdate);
    QVERIFY(job.canStart());
}

#include "moc_syncmessagesjobtest.cpp"
