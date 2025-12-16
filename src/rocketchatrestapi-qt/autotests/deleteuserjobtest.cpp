/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deleteuserjobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "users/deleteuserjob.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(DeleteUserJobTest)
using namespace RocketChatRestApi;
DeleteUserJobTest::DeleteUserJobTest(QObject *parent)
    : QObject(parent)
{
}

void DeleteUserJobTest::shouldHaveDefaultValue()
{
    DeleteUserJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasUserIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void DeleteUserJobTest::shouldGenerateRequest()
{
    DeleteUserJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.delete"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void DeleteUserJobTest::shouldGenerateJson()
{
    DeleteUserJob job;
    UserBaseJob::UserInfo info;
    info.userIdentifier = u"foo1"_s;
    info.userInfoType = UserBaseJob::UserInfoType::UserId;
    job.setUserInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"userId":"%1"})").arg(info.userIdentifier).toLatin1());
}

void DeleteUserJobTest::shouldNotStarting()
{
    DeleteUserJob job;

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
    UserBaseJob::UserInfo info;
    info.userIdentifier = u"foo1"_s;
    info.userInfoType = UserBaseJob::UserInfoType::UserName;
    job.setUserInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_deleteuserjobtest.cpp"
