/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setuseractivestatusjobtest.h"

#include "ruqola_restapi_helper.h"

#include "users/setuseractivestatusjob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(SetUserActiveStatusJobTest)
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SetUserActiveStatusJobTest::SetUserActiveStatusJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetUserActiveStatusJobTest::shouldHaveDefaultValue()
{
    SetUserActiveStatusJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.activateUserId().isEmpty());
    QVERIFY(job.activate());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SetUserActiveStatusJobTest::shouldGenerateRequest()
{
    SetUserActiveStatusJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.setActiveStatus"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SetUserActiveStatusJobTest::shouldGenerateJson()
{
    SetUserActiveStatusJob job;
    const QByteArray userId("foo1");
    job.setActivateUserId(userId);
    job.setActivate(false);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"activeStatus":false,"userId":"%1"})"_s.arg(QLatin1StringView(userId)).toLatin1());
    job.setActivate(true);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"activeStatus":true,"userId":"%1"})"_s.arg(QLatin1StringView(userId)).toLatin1());
}

void SetUserActiveStatusJobTest::shouldNotStarting()
{
    SetUserActiveStatusJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QByteArray statusUserid("foo1");
    job.setActivateUserId(statusUserid);
    QVERIFY(job.canStart());
}

#include "moc_setuseractivestatusjobtest.cpp"
