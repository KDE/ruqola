/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelgetallusermentionsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/channelgetallusermentionsjob.h"

#include "ruqola_restapi_helper.h"
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
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void ChannelGetAllUserMentionsJobTest::shouldHaveParameterSupport()
{
    ChannelGetAllUserMentionsJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QString roomId = u"avat"_s;
    job.setRoomId(roomId);
    QueryParameters parameters;
    parameters.setCount(5);
    parameters.setOffset(12);
    job.setQueryParameters(parameters);
    QNetworkRequest request = job.request();
    verifyAuthentication(&job, request);
    QCOMPARE(request.url().toString(), u"http://www.kde.org/api/v1/channels.getAllUserMentionsByChannel?roomId=avat&count=5&offset=12"_s);
}

void ChannelGetAllUserMentionsJobTest::shouldHaveParameterSupportSorting()
{
    ChannelGetAllUserMentionsJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QString roomId = u"avat"_s;
    job.setRoomId(roomId);
    QueryParameters parameters;
    parameters.setCount(5);
    parameters.setOffset(12);

    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(u"foo"_s, QueryParameters::SortOrder::Descendant);
    parameters.setSorting(map);

    job.setQueryParameters(parameters);
    QNetworkRequest request = job.request();
    verifyAuthentication(&job, request);
    QCOMPARE(request.url().toString(),
             u"http://www.kde.org/api/v1/channels.getAllUserMentionsByChannel?roomId=avat&count=5&offset=12&sort=%7B%22foo%22:-1%7D"_s);
}

void ChannelGetAllUserMentionsJobTest::shouldHaveParameterSupportSortingTwoParameters()
{
    ChannelGetAllUserMentionsJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QString roomId = u"avat"_s;
    job.setRoomId(roomId);
    QueryParameters parameters;
    parameters.setCount(5);
    parameters.setOffset(12);

    QMap<QString, QueryParameters::SortOrder> map;
    map.insert(u"foo"_s, QueryParameters::SortOrder::Descendant);
    map.insert(u"bla"_s, QueryParameters::SortOrder::Ascendant);
    parameters.setSorting(map);

    job.setQueryParameters(parameters);
    QNetworkRequest request = job.request();
    verifyAuthentication(&job, request);
    QCOMPARE(request.url().toString(),
             u"http://www.kde.org/api/v1/channels.getAllUserMentionsByChannel?roomId=avat&count=5&offset=12&sort=%7B%22bla%22:1,%22foo%22:-1%7D"_s);
}

void ChannelGetAllUserMentionsJobTest::shouldGenerateRequest()
{
    ChannelGetAllUserMentionsJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QString roomId = u"avat"_s;
    job.setRoomId(roomId);
    QNetworkRequest request = job.request();
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.getAllUserMentionsByChannel?roomId=avat"_s));
}

void ChannelGetAllUserMentionsJobTest::shouldNotStarting()
{
    ChannelGetAllUserMentionsJob job;

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
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_channelgetallusermentionsjobtest.cpp"
