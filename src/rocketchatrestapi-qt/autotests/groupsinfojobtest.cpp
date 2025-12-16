/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupsinfojobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/groupsinfojob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include <QTest>

QTEST_GUILESS_MAIN(GroupsInfoJobTest)
using namespace RocketChatRestApi;
GroupsInfoJobTest::GroupsInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void GroupsInfoJobTest::shouldHaveDefaultValue()
{
    GroupsInfoJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GroupsInfoJobTest::shouldGenerateRequest()
{
    GroupsInfoJob job;
    job.setRoomId(u"foo"_s);
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/groups.info?roomId=foo"_s));
}

void GroupsInfoJobTest::shouldNotStarting()
{
    GroupsInfoJob job;

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

#include "moc_groupsinfojobtest.cpp"
