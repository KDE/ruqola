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

#include "videoconfupdatejitsitimeoutjobtest.h"
#include "video-conference/videoconfupdatejitsitimeoutjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(VideoConfUpdateJitsiTimeOutJobTest)
using namespace RocketChatRestApi;
VideoConfUpdateJitsiTimeOutJobTest::VideoConfUpdateJitsiTimeOutJobTest(QObject *parent)
    : QObject(parent)
{
}

void VideoConfUpdateJitsiTimeOutJobTest::shouldHaveDefaultValue()
{
    VideoConfUpdateJitsiTimeOutJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void VideoConfUpdateJitsiTimeOutJobTest::shouldGenerateRequest()
{
    VideoConfUpdateJitsiTimeOutJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/video-conference/jitsi.update-timeout")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void VideoConfUpdateJitsiTimeOutJobTest::shouldGenerateJson()
{
    VideoConfUpdateJitsiTimeOutJob job;
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"roomId\":\"%1\"}").arg(roomId).toLatin1());
}

void VideoConfUpdateJitsiTimeOutJobTest::shouldNotStarting()
{
    VideoConfUpdateJitsiTimeOutJob job;

    auto *method = new RestApiMethod;
    method->setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(method);

    auto *mNetworkAccessManager = new QNetworkAccessManager;
    job.setNetworkAccessManager(mNetworkAccessManager);
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

    delete method;
    delete mNetworkAccessManager;
}
