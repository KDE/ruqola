/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setavatarjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "users/setavatarjob.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(SetAvatarJobTest)
using namespace RocketChatRestApi;
SetAvatarJobTest::SetAvatarJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetAvatarJobTest::shouldHaveDefaultValue()
{
    SetAvatarJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.avatarInfo().isValid());
    QVERIFY(!job.hasUserIdentifier());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SetAvatarJobTest::shouldGenerateRequest()
{
    {
        SetAvatarJob job;

        // We need it otherwise application/json is not defined as we can send as json or multipart
        const QString avatarurl = u"http://www.kde.org"_s;
        SetAvatarJob::SetAvatarInfo avatarInfo;
        avatarInfo.mAvatarUrl = avatarurl;
        job.setAvatarInfo(avatarInfo);
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.setAvatar"_s));
        QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
    }
    {
        SetAvatarJob job;

        // We need it otherwise application/json is not defined as we can send as json or multipart
        const QString avatarurl = u"http://www.kde.org"_s;
        SetAvatarJob::SetAvatarInfo avatarInfo;
        avatarInfo.mImageUrl = QUrl::fromUserInput(avatarurl);
        job.setAvatarInfo(avatarInfo);
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.setAvatar"_s));
        QVERIFY(request.header(QNetworkRequest::ContentTypeHeader).toString() != u"application/json"_s);
    }
}

void SetAvatarJobTest::shouldGenerateJson()
{
    SetAvatarJob job;

    const QString avatarurl = u"foo1"_s;

    SetAvatarJob::SetAvatarInfo avatarInfo;
    avatarInfo.mAvatarUrl = avatarurl;
    job.setAvatarInfo(avatarInfo);

    UserBaseJob::UserInfo info;
    info.userIdentifier = u"foo"_s;
    info.userInfoType = UserBaseJob::UserInfoType::UserId;
    job.setUserInfo(info);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"avatarUrl":"%1","userId":"%2"})").arg(avatarurl, info.userIdentifier).toLatin1());
}

void SetAvatarJobTest::shouldNotStarting()
{
    SetAvatarJob job;

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

    const QString avatarurl = u"foo1"_s;
    SetAvatarJob::SetAvatarInfo avatarInfo;
    avatarInfo.mAvatarUrl = avatarurl;
    job.setAvatarInfo(avatarInfo);

    QVERIFY(!job.canStart());
    UserBaseJob::UserInfo info;
    info.userIdentifier = u"foo"_s;
    info.userInfoType = UserBaseJob::UserInfoType::UserId;
    job.setUserInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_setavatarjobtest.cpp"
