/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getpresencejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
GetPresenceJob::GetPresenceJob(QObject *parent)
    : UserBaseJob(parent)
{
}

GetPresenceJob::~GetPresenceJob() = default;

bool GetPresenceJob::canStart() const
{
    if (!hasUserIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetPresenceJob: identifier is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool GetPresenceJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("GetPresenceJob ask for presenceUserId: " + mUserInfo.userIdentifier.toLatin1());
    return true;
}

void GetPresenceJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GetPresenceJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getPresenceDone(replyObject["presence"_L1].toString());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GetPresenceJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest GetPresenceJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersGetPresence);
    addQueryUrl(url);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool GetPresenceJob::requireHttpAuthentication() const
{
    return true;
}

#include "moc_getpresencejob.cpp"
