/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetavatarjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
ResetAvatarJob::ResetAvatarJob(QObject *parent)
    : UserBaseJob(parent)
{
}

ResetAvatarJob::~ResetAvatarJob() = default;

bool ResetAvatarJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ResetAvatarJob::start"_ba);
    submitPostRequest(json());
    return true;
}

void ResetAvatarJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (checkResponse("ResetAvatarJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT resetAvatarDone();
    }
}

bool ResetAvatarJob::requireHttpAuthentication() const
{
    return true;
}

bool ResetAvatarJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!hasUserIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ResetAvatarJob: identifier is empty";
        return false;
    }
    return true;
}

QNetworkRequest ResetAvatarJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersResetAvatar);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ResetAvatarJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_resetavatarjob.cpp"
