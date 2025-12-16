/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "listinvitejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "invite/listinvitejob.h"

#include "ruqola_restapi_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(ListInviteJobTest)
using namespace RocketChatRestApi;
ListInviteJobTest::ListInviteJobTest(QObject *parent)
    : QObject(parent)
{
}

void ListInviteJobTest::shouldHaveDefaultValue()
{
    ListInviteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void ListInviteJobTest::shouldGenerateRequest()
{
    ListInviteJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/listInvites"_s));
    }
}

void ListInviteJobTest::shouldNotStarting()
{
    ListInviteJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager networkAccessManager;
    job.setNetworkAccessManager(&networkAccessManager);
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    job.setUserId(userId);
    QVERIFY(job.canStart());
}

#include "moc_listinvitejobtest.cpp"
