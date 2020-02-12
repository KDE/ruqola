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

#include "channelinfojobtest.h"
#include "channels/channelinfojob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(ChannelInfoJobTest)
using namespace RocketChatRestApi;
ChannelInfoJobTest::ChannelInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelInfoJobTest::shouldHaveDefaultValue()
{
    ChannelInfoJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasRoomIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChannelInfoJobTest::shouldGenerateRequest()
{
    ChannelInfoJob job;
    ChannelBaseJob::ChannelInfo info;
    info.channelInfoType = ChannelBaseJob::ChannelInfoType::RoomId;
    info.channelInfoIdentifier = QStringLiteral("foo");
    job.setChannelInfo(info);

    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.info?roomId=foo")));
}

void ChannelInfoJobTest::shouldNotStarting()
{
    ChannelInfoJob job;

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
    ChannelBaseJob::ChannelInfo info;
    info.channelInfoType = ChannelBaseJob::ChannelInfoType::RoomId;
    info.channelInfoIdentifier = roomId;
    job.setChannelInfo(info);
    QVERIFY(job.canStart());
}
