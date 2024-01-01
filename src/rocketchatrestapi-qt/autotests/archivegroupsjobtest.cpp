/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "archivegroupsjobtest.h"
#include "groups/archivegroupsjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ArchiveGroupsJobTest)
using namespace RocketChatRestApi;
ArchiveGroupsJobTest::ArchiveGroupsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ArchiveGroupsJobTest::shouldHaveDefaultValue()
{
    ArchiveGroupsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ArchiveGroupsJobTest::shouldGenerateRequest()
{
    ArchiveGroupsJob job;
    job.setArchive(true);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.archive")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));

    job.setArchive(false);
    request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.unarchive")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ArchiveGroupsJobTest::shouldGenerateJson()
{
    ArchiveGroupsJob job;
    const QString roomId = QStringLiteral("foo1");
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(roomId).toLatin1());
}

void ArchiveGroupsJobTest::shouldNotStarting()
{
    ArchiveGroupsJob job;

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
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_archivegroupsjobtest.cpp"
