/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsnamejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/changegroupsnamejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeGroupsNameJobTest)
using namespace RocketChatRestApi;
ChangeGroupsNameJobTest::ChangeGroupsNameJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsNameJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsNameJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.name().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeGroupsNameJobTest::shouldGenerateRequest()
{
    ChangeGroupsNameJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.rename"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChangeGroupsNameJobTest::shouldGenerateJson()
{
    ChangeGroupsNameJob job;
    const QString roomId = u"foo1"_s;
    const QString name = u"topic1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setName(name);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"name":"%1","roomId":"%2"})").arg(name, roomId).toLatin1());
}

#include "moc_changegroupsnamejobtest.cpp"
