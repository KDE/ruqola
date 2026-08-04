/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fetchuserswaitingforgroupkeyjob.h"

#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
FetchUsersWaitingForGroupKeyJob::FetchUsersWaitingForGroupKeyJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

FetchUsersWaitingForGroupKeyJob::~FetchUsersWaitingForGroupKeyJob() = default;

bool FetchUsersWaitingForGroupKeyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("Start FetchUsersWaitingForGroupKeyJob");

    return true;
}

void FetchUsersWaitingForGroupKeyJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("FetchUsersWaitingForGroupKeyJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT fetchUsersWaitingForGroupKeyDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("FetchUsersWaitingForGroupKeyJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray FetchUsersWaitingForGroupKeyJob::getRoomId() const
{
    return roomId;
}

void FetchUsersWaitingForGroupKeyJob::setRoomId(const QByteArray &newRoomId)
{
    roomId = newRoomId;
}

QNetworkRequest FetchUsersWaitingForGroupKeyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EFetchUsersWaitingForGroupKey);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool FetchUsersWaitingForGroupKeyJob::requireHttpAuthentication() const
{
    return true;
}

#include "moc_fetchuserswaitingforgroupkeyjob.cpp"
