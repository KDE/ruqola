/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechannelnamejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ChangeChannelNameJob::ChangeChannelNameJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeChannelNameJob::~ChangeChannelNameJob() = default;

bool ChangeChannelNameJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeChannelNameJob::start");
    submitPostRequest(json());
    return true;
}

void ChangeChannelNameJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("Change name success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeNameDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("Problem when we tried to change name: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChangeChannelNameJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeChannelNameJob::canStart() const
{
    if (mName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeChannelNameJob: name is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeChannelNameJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeChannelNameJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = roomId();
    jsonObj["name"_L1] = name();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeChannelNameJob::roomId() const
{
    return mRoomId;
}

void ChangeChannelNameJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString ChangeChannelNameJob::name() const
{
    return mName;
}

void ChangeChannelNameJob::setName(const QString &t)
{
    mName = t;
}

QNetworkRequest ChangeChannelNameJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsRename);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_changechannelnamejob.cpp"
