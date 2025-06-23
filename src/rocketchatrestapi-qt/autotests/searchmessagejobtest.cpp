/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/searchmessagejob.h"
#include "ruqola_restapi_helper.h"
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
    QCOMPARE(!job.offset(), 0);
}

void searchMessageJobTest::shouldGenerateRequest()
{
    SearchMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setRoomId(u"foo"_s);
    job.setSearchText(u"bla"_s);
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.search?roomId=foo&searchText=bla"_s));
}

void searchMessageJobTest::shouldGenerateRequestWithLimit()
{
    SearchMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    job.setRoomId(u"foo"_s);
    job.setSearchText(u"bla"_s);
    job.setCount(5);
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.search?roomId=foo&searchText=bla&count=5"_s));
}

#include "moc_searchmessagejobtest.cpp"
