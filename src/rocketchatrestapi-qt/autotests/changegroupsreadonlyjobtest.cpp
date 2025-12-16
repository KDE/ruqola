/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupsreadonlyjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/changegroupsreadonlyjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeGroupsReadonlyJobTest)
using namespace RocketChatRestApi;
ChangeGroupsReadonlyJobTest::ChangeGroupsReadonlyJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsReadonlyJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsReadonlyJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(!job.readOnly());
    QVERIFY(!job.hasIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeGroupsReadonlyJobTest::shouldGenerateRequest()
{
    ChangeGroupsReadonlyJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.setReadOnly"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChangeGroupsReadonlyJobTest::shouldGenerateJson()
{
    ChangeGroupsReadonlyJob job;
    const QString roomId = u"foo1"_s;
    bool readOnly = true;
    ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = roomId;
    job.setChannelGroupInfo(info);
    job.setReadOnly(readOnly);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"readOnly":true,"roomId":"%1"})").arg(roomId).toLatin1());

    readOnly = false;
    job.setReadOnly(readOnly);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"readOnly":false,"roomId":"%1"})").arg(roomId).toLatin1());
}

#include "moc_changegroupsreadonlyjobtest.cpp"
