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

#include "channelcleanhistoryjobtest.h"
#include "channels/channelcleanhistoryjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChannelCleanHistoryJobTest)
using namespace RocketChatRestApi;
ChannelCleanHistoryJobTest::ChannelCleanHistoryJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelCleanHistoryJobTest::shouldHaveDefaultValue()
{
    ChannelCleanHistoryJob job;
    QVERIFY(!job.cleanHistoryInfo().isValid());
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelCleanHistoryJobTest::shouldGenerateRequest()
{
    ChannelCleanHistoryJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.cleanHistory")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChannelCleanHistoryJobTest::shouldGenerateJson()
{
    ChannelCleanHistoryJob job;
    ChannelCleanHistoryJob::CleanHistoryInfo info;
    const QString roomId = QStringLiteral("room1");
    info.roomId = roomId;
    info.latest = QDateTime(QDate(2020, 12, 3), QTime(5, 7, 50));
    info.oldest = QDateTime(QDate(2020, 3, 3), QTime(5, 7, 50));
    job.setCleanHistoryInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"roomId\":\"%1\"}").arg(roomId).toLatin1());
}

void ChannelCleanHistoryJobTest::shouldNotStarting()
{
    ChannelCleanHistoryJob job;

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
    ChannelCleanHistoryJob::CleanHistoryInfo info;
    const QString roomId = QStringLiteral("room1");
    info.roomId = roomId;
    job.setCleanHistoryInfo(info);
    QVERIFY(!job.canStart());

    info.latest = QDateTime(QDate(2020, 12, 3), QTime(5, 7, 50));
    info.oldest = QDateTime(QDate(2020, 3, 3), QTime(5, 7, 50));
    job.setCleanHistoryInfo(info);
    QVERIFY(job.canStart());
}
