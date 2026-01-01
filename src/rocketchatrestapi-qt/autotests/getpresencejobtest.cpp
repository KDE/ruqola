/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getpresencejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "users/getpresencejob.h"
#include <QTest>

QTEST_GUILESS_MAIN(GetPresenceJobTest)
using namespace RocketChatRestApi;
GetPresenceJobTest::GetPresenceJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetPresenceJobTest::shouldHaveDefaultValue()
{
    GetPresenceJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasUserIdentifier());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GetPresenceJobTest::shouldGenerateRequest()
{
    GetPresenceJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    UserBaseJob::UserInfo info;
    info.userIdentifier = u"foo"_s;
    info.userInfoType = UserBaseJob::UserInfoType::UserId;
    job.setUserInfo(info);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.getPresence?userId=%1"_s.arg(info.userIdentifier)));
}

#include "moc_getpresencejobtest.cpp"
