/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ignoreuserjobtest.h"
#include "chat/ignoreuserjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(IgnoreUserJobTest)
using namespace RocketChatRestApi;
IgnoreUserJobTest::IgnoreUserJobTest(QObject *parent)
    : QObject(parent)
{
}

void IgnoreUserJobTest::shouldHaveDefaultValue()
{
    IgnoreUserJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.ignoreUserId().isEmpty());
    QVERIFY(job.ignore());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void IgnoreUserJobTest::shouldGenerateRequest()
{
    IgnoreUserJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setRoomId(QStringLiteral("foo"));
    job.setIgnoreUserId(QStringLiteral("bla"));
    verifyAuthentication(&job, request);
    // TODO verify
    // TODO add ignore == false
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.ignoreUser?rid=foo&userId=bla&ignore=true")));
}

#include "moc_ignoreuserjobtest.cpp"
