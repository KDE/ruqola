/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupsinfojobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "groups/groupsinfojob.h"
#include "ruqola_restapi_helper.h"

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

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_groupsinfojobtest.cpp"
