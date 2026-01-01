/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsencryptedjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/changegroupsencryptedjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeGroupsEncryptedJobTest)
using namespace RocketChatRestApi;
ChangeGroupsEncryptedJobTest::ChangeGroupsEncryptedJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsEncryptedJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsEncryptedJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(!job.encrypted());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeGroupsEncryptedJobTest::shouldGenerateRequest()
{
    ChangeGroupsEncryptedJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.setEncrypted"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChangeGroupsEncryptedJobTest::shouldGenerateJson()
{
    ChangeGroupsEncryptedJob job;
    const QString roomId = u"foo1"_s;
    bool encrypted = true;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setEncrypted(encrypted);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"encrypted":true,"roomId":"%1"})").arg(roomId).toLatin1());

    encrypted = false;
    job.setEncrypted(encrypted);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"encrypted":false,"roomId":"%1"})").arg(roomId).toLatin1());
}

#include "moc_changegroupsencryptedjobtest.cpp"
