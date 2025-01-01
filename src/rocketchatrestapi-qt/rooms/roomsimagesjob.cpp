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

    addStartRestApiInfo(QByteArrayLiteral("RoomsImagesJob: Ask discussions in room"));
    return true;
}

void RoomsImagesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RoomsImagesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsImagesDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RoomsImagesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
    queryUrl.addQueryItem(QStringLiteral("roomId"), QLatin1StringView(mRoomsImagesJobInfo.roomId));
    queryUrl.addQueryItem(QStringLiteral("startingFromId"), QLatin1StringView(mRoomsImagesJobInfo.startingFromId));
    queryUrl.addQueryItem(QStringLiteral("offset"), QString::number(mRoomsImagesJobInfo.offset));
    queryUrl.addQueryItem(QStringLiteral("count"), QString::number(mRoomsImagesJobInfo.count));
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

#include "moc_roomsimagesjob.cpp"
