/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechannelreadonlyjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
ChangeChannelReadonlyJob::ChangeChannelReadonlyJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeChannelReadonlyJob::~ChangeChannelReadonlyJob() = default;

bool ChangeChannelReadonlyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeChannelReadonlyJob::start");
    submitPostRequest(json());
    return true;
}

void ChangeChannelReadonlyJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("Change read only success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeReadonlyDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerInfo("Problem when we tried to change read only status: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChangeChannelReadonlyJob::readOnly() const
{
    return mReadOnly;
}

void ChangeChannelReadonlyJob::setReadOnly(bool readOnly)
{
    mReadOnly = readOnly;
}

bool ChangeChannelReadonlyJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeChannelReadonlyJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeChannelReadonlyJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeChannelReadonlyJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = roomId();
    jsonObj["readOnly"_L1] = readOnly();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeChannelReadonlyJob::roomId() const
{
    return mRoomId;
}

void ChangeChannelReadonlyJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChangeChannelReadonlyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetReadOnly);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_changechannelreadonlyjob.cpp"
