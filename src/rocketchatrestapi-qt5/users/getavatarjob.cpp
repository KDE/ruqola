/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getavatarjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>

#include <QNetworkReply>
using namespace RocketChatRestApi;
GetAvatarJob::GetAvatarJob(QObject *parent)
    : UserBaseJob(parent)
{
}

GetAvatarJob::~GetAvatarJob() = default;

bool GetAvatarJob::canStart() const
{
    if (!hasUserIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetAvatarJob: identifier is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool GetAvatarJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }

    submitGetRequest();
    addStartRestApiInfo("GetAvatarJob ask for avatarUserId: " + mUserInfo.userIdentifier.toLatin1());
    addStartRestApiInfo("GetAvatarJob::start");
    return true;
}

void GetAvatarJob::onGetRequestResponse(const QJsonDocument &)
{
    const QString userId = mUserInfo.userIdentifier;
    if (!mReply->error()) {
        const QUrl url = mReply->url();
        if (url.isValid() && !url.scheme().isEmpty()) {
            addLoggerInfo(QByteArrayLiteral("GetAvatarJob success: ") + userId.toUtf8());
            Q_EMIT avatar(mUserInfo, url);
        } else {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << "expected a URL, got something else:";
        }
    } else {
        addLoggerWarning(QByteArrayLiteral("GetAvatarJob error: ") + userId.toUtf8());
        Q_EMIT avatar(mUserInfo, QUrl());
    }
}

QNetworkRequest GetAvatarJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersGetAvatar);
    addQueryUrl(url);
    QNetworkRequest request(url);
    return request;
}

bool GetAvatarJob::requireHttpAuthentication() const
{
    return false;
}

QString GetAvatarJob::jobName() const
{
    return i18n("Get Avatar");
}
