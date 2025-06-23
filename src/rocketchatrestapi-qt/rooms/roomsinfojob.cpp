/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsinfojob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RoomsInfoJob::RoomsInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsInfoJob::~RoomsInfoJob() = default;

bool RoomsInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsInfoJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsInfoJob: mRoomId is not valid.";
        return false;
    }

    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool RoomsInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start roomsinfo job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("RoomsInfoJob: Ask info about rooms"_ba);
    return true;
}

void RoomsInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsInfoJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsInfoDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsInfoJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString RoomsInfoJob::roomId() const
{
    return mRoomId;
}

void RoomsInfoJob::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

QNetworkRequest RoomsInfoJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsInfo);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"roomId"_s, mRoomId);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request);

    addAuthRawHeader(request);
    return request;
}

#include "moc_roomsinfojob.cpp"
