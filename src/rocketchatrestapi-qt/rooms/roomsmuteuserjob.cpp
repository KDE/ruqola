/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsmuteuserjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
RoomsMuteUserJob::RoomsMuteUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsMuteUserJob::~RoomsMuteUserJob() = default;

bool RoomsMuteUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsMuteUserJob::start");
    submitPostRequest(json());

    return true;
}

void RoomsMuteUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RoomsMuteUserJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT cleanHistoryDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RoomsMuteUserJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool RoomsMuteUserJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsMuteUserJob::canStart() const
{
    // if (!mCleanHistoryInfo.isValid()) {
    //     qCWarning(ROCKETCHATQTRESTAPI_LOG) << "mCleanHistoryInfo: mCleanHistoryInfo is not valid.";
    //     return false;
    // }

    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument RoomsMuteUserJob::json() const
{
    QJsonObject jsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData**************** " << postData;
    return postData;
}

QNetworkRequest RoomsMuteUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsCleanHistory);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_roomsmuteuserjob.cpp"
