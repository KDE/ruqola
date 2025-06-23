/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsimagesjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RoomsImagesJob::RoomsImagesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsImagesJob::~RoomsImagesJob() = default;

bool RoomsImagesJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsImagesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mRoomsImagesJobInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsImagesJob: mRoomsImagesJobInfo is empty";
        return false;
    }
    return true;
}

bool RoomsImagesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RoomsImagesJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("RoomsImagesJob: Ask discussions in room"_ba);
    return true;
}

void RoomsImagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsImagesJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsImagesDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsImagesJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

RoomsImagesJob::RoomsImagesJobInfo RoomsImagesJob::roomsImagesJobInfo() const
{
    return mRoomsImagesJobInfo;
}

void RoomsImagesJob::setRoomsImagesJobInfo(const RoomsImagesJobInfo &newRoomsImagesJobInfo)
{
    mRoomsImagesJobInfo = newRoomsImagesJobInfo;
}

bool RoomsImagesJob::hasQueryParameterSupport() const
{
    return true;
}

QNetworkRequest RoomsImagesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsImages);
    QUrlQuery queryUrl;
    mRoomsImagesJobInfo.generateRequest(queryUrl);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request);

    addAuthRawHeader(request);
    return request;
}

bool RoomsImagesJob::RoomsImagesJobInfo::isValid() const
{
    return !roomId.isEmpty() && !startingFromId.isEmpty() && (count > 0);
}

void RoomsImagesJob::RoomsImagesJobInfo::generateRequest(QUrlQuery &query) const
{
    query.addQueryItem(u"roomId"_s, QLatin1StringView(roomId));
    query.addQueryItem(u"startingFromId"_s, QLatin1StringView(startingFromId));
    query.addQueryItem(u"offset"_s, QString::number(offset));
    query.addQueryItem(u"count"_s, QString::number(count));
}

#include "moc_roomsimagesjob.cpp"
