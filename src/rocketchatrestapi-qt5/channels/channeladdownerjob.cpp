/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeladdownerjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelAddOwnerJob::ChannelAddOwnerJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelAddOwnerJob::~ChannelAddOwnerJob() = default;

bool ChannelAddOwnerJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelAddOwnerJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelAddOwnerJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("Add owner success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT addOwnerDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("Problem when we tried to add owner: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ChannelAddOwnerJob::addownerUserId() const
{
    return mAddownerUserId;
}

void ChannelAddOwnerJob::setAddownerUserId(const QString &addownerUserId)
{
    mAddownerUserId = addownerUserId;
}

bool ChannelAddOwnerJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelAddOwnerJob::canStart() const
{
    if (mAddownerUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelAddOwnerJob: remove userid is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelAddOwnerJob: RoomId and roomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelAddOwnerJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj[QLatin1String("userId")] = addownerUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelAddOwnerJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddOwner);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_channeladdownerjob.cpp"
