/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechanneldescriptionjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
ChangeChannelDescriptionJob::ChangeChannelDescriptionJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeChannelDescriptionJob::~ChangeChannelDescriptionJob() = default;

bool ChangeChannelDescriptionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeChannelDescriptionJob::start");
    submitPostRequest(json());
    return true;
}

void ChangeChannelDescriptionJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("Change description success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeDescriptionDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("Problem when we tried to change description: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ChangeChannelDescriptionJob::description() const
{
    return mDescription;
}

void ChangeChannelDescriptionJob::setDescription(const QString &description)
{
    mDescription = description;
}

bool ChangeChannelDescriptionJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeChannelDescriptionJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeChannelDescriptionJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeChannelDescriptionJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = roomId();
    jsonObj["description"_L1] = description();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeChannelDescriptionJob::roomId() const
{
    return mRoomId;
}

void ChangeChannelDescriptionJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChangeChannelDescriptionJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetDescription);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_changechanneldescriptionjob.cpp"
