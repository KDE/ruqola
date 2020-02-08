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

#include "channelgetallusermentionsjobtest.h"
#include "channels/channelgetallusermentionsjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(ChannelGetAllUserMentionsJobTest)
using namespace RocketChatRestApi;
ChannelGetAllUserMentionsJobTest::ChannelGetAllUserMentionsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelGetAllUserMentionsJobTest::shouldHaveDefaultValue()
{
    ChannelGetAllUserMentionsJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
}

void ChannelGetAllUserMentionsJobTest::shouldHaveParameterSupport()
{
    ChannelGetAllUserMentionsJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString roomId = QStringLiteral("avat");
    job.setRoomId(roomId);
    QueryParameters parameters;
    parameters.setCount(5);
    parameters.setOffset(12);
    job.setQueryParameters(parameters);
    QNetworkRequest request = job.request();
    verifyAuthentication(&job, request);
    QCOMPARE(request.url().toString(), QStringLiteral("http://www.kde.org/api/v1/channels.getAllUserMentionsByChannel?roomId=avat&count=5&offset=12"));
}

void ChannelGetAllUserMentionsJobTest::shouldHaveParameterSupportSorting()
{
    ChannelGetAllUserMentionsJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString roomId = QStringLiteral("avat");
    job.setRoomId(roomId);
    QueryParameters parameters;
    parameters.setCount(5);
    parameters.setOffset(12);

    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("foo"), QueryParameters::SortOrder::Descendant);
    parameters.setSorting(map);

    job.setQueryParameters(parameters);
    QNetworkRequest request = job.request();
    verifyAuthentication(&job, request);
    QCOMPARE(request.url().toString(), QStringLiteral("http://www.kde.org/api/v1/channels.getAllUserMentionsByChannel?roomId=avat&count=5&offset=12&sort=%7B%22foo%22:-1%7D"));
}

void ChannelGetAllUserMentionsJobTest::shouldHaveParameterSupportSortingTwoParameters()
{
    ChannelGetAllUserMentionsJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString roomId = QStringLiteral("avat");
    job.setRoomId(roomId);
    QueryParameters parameters;
    parameters.setCount(5);
    parameters.setOffset(12);

    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("foo"), QueryParameters::SortOrder::Descendant);
    map.insert(QStringLiteral("bla"), QueryParameters::SortOrder::Ascendant);
    parameters.setSorting(map);

    job.setQueryParameters(parameters);
    QNetworkRequest request = job.request();
    verifyAuthentication(&job, request);
    QCOMPARE(request.url().toString(), QStringLiteral("http://www.kde.org/api/v1/channels.getAllUserMentionsByChannel?roomId=avat&count=5&offset=12&sort=%7B%22bla%22:1,%22foo%22:-1%7D"));
}

void ChannelGetAllUserMentionsJobTest::shouldGenerateRequest()
{
    ChannelGetAllUserMentionsJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString roomId = QStringLiteral("avat");
    job.setRoomId(roomId);
    QNetworkRequest request = job.request();
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.getAllUserMentionsByChannel?roomId=avat")));
}

void ChannelGetAllUserMentionsJobTest::shouldNotStarting()
{
    ChannelGetAllUserMentionsJob job;

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
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}
