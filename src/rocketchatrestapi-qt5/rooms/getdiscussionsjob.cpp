/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getdiscussionsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
GetDiscussionsJob::GetDiscussionsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetDiscussionsJob::~GetDiscussionsJob() = default;

bool GetDiscussionsJob::requireHttpAuthentication() const
{
    return true;
}

bool GetDiscussionsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetDiscussionsJob: mRoomId is empty";
        return false;
    }
    return true;
}

bool GetDiscussionsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetDiscussionsJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("GetDiscussionsJob: Ask discussions in room"));
    return true;
}

void GetDiscussionsJob::onGetRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetDiscussionsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getDiscussionsDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("GetDiscussionsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GetDiscussionsJob::roomId() const
{
    return mRoomId;
}

void GetDiscussionsJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool GetDiscussionsJob::hasQueryParameterSupport() const
{
    return true;
}

QNetworkRequest GetDiscussionsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsGetDiscussions);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request, false);

    addAuthRawHeader(request);
    return request;
}
