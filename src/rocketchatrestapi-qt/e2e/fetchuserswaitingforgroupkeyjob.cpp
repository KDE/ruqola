/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fetchuserswaitingforgroupkeyjob.h"

#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrlQuery>

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

    addStartRestApiInfo("Start FetchUsersWaitingForGroupKeyJob"_ba);

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

QByteArray FetchUsersWaitingForGroupKeyJob::roomId() const
{
    return mRoomIds.isEmpty() ? QByteArray{} : mRoomIds.constFirst();
}

void FetchUsersWaitingForGroupKeyJob::setRoomId(const QByteArray &newRoomId)
{
    mRoomIds = {newRoomId};
}

QList<QByteArray> FetchUsersWaitingForGroupKeyJob::roomIds() const
{
    return mRoomIds;
}

void FetchUsersWaitingForGroupKeyJob::setRoomIds(const QList<QByteArray> &newRoomIds)
{
    mRoomIds = newRoomIds;
}

QNetworkRequest FetchUsersWaitingForGroupKeyJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EFetchUsersWaitingForGroupKey);
    QUrlQuery queryUrl;
    for (const QByteArray &roomIdentifier : mRoomIds) {
        queryUrl.addQueryItem(u"roomIds[]"_s, QLatin1StringView(roomIdentifier));
    }
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool FetchUsersWaitingForGroupKeyJob::requireHttpAuthentication() const
{
    return true;
}

#include "moc_fetchuserswaitingforgroupkeyjob.cpp"
