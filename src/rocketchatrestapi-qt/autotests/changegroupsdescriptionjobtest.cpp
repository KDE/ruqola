/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsdescriptionjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/changegroupsdescriptionjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeGroupsDescriptionJobTest)
using namespace RocketChatRestApi;
ChangeGroupsDescriptionJobTest::ChangeGroupsDescriptionJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsDescriptionJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsDescriptionJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.description().isEmpty());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeGroupsDescriptionJobTest::shouldGenerateRequest()
{
    ChangeGroupsDescriptionJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.setDescription"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChangeGroupsDescriptionJobTest::shouldGenerateJson()
{
    ChangeGroupsDescriptionJob job;
    const QString roomId = u"foo1"_s;
    const QString description = u"topic1"_s;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setDescription(description);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"description":"%1","roomId":"%2"})").arg(description, roomId).toLatin1());
}

#include "moc_changegroupsdescriptionjobtest.cpp"
