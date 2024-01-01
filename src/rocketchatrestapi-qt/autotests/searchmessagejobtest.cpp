/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagejobtest.h"
#include "chat/searchmessagejob.h"
#include "ruqola_restapi_helper.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(searchMessageJobTest)
using namespace RocketChatRestApi;
searchMessageJobTest::searchMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void searchMessageJobTest::shouldHaveDefaultValue()
{
    SearchMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.searchText().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    QCOMPARE(job.count(), -1);
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.useRegularExpression());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void searchMessageJobTest::shouldGenerateRequest()
{
    SearchMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setRoomId(QStringLiteral("foo"));
    job.setSearchText(QStringLiteral("bla"));
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.search?roomId=foo&searchText=bla")));
}

void searchMessageJobTest::shouldGenerateRequestWithLimit()
{
    SearchMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setRoomId(QStringLiteral("foo"));
    job.setSearchText(QStringLiteral("bla"));
    job.setCount(5);
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.search?roomId=foo&searchText=bla&count=5")));
}

#include "moc_searchmessagejobtest.cpp"
