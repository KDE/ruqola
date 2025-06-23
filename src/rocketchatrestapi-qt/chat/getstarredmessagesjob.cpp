/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getstarredmessagesjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
GetStarredMessagesJob::GetStarredMessagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetStarredMessagesJob::~GetStarredMessagesJob() = default;

bool GetStarredMessagesJob::requireHttpAuthentication() const
{
    return true;
}

bool GetStarredMessagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetStarredMessagesJob: mRoomId is empty";
        return false;
    }
    return true;
}

bool GetStarredMessagesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetStarredMessagesJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("GetStarredMessagesJob: Ask starred messages"_ba);
    return true;
}

void GetStarredMessagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GetStarredMessagesJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getStarredMessagesDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GetStarredMessagesJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray GetStarredMessagesJob::roomId() const
{
    return mRoomId;
}

void GetStarredMessagesJob::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest GetStarredMessagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatGetStarredMessages);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"roomId"_s, QLatin1StringView(mRoomId));
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request);

    addAuthRawHeader(request);
    return request;
}

bool GetStarredMessagesJob::hasQueryParameterSupport() const
{
    return true;
}

#include "moc_getstarredmessagesjob.cpp"
