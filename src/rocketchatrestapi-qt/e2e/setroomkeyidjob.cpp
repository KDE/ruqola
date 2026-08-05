/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setroomkeyidjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SetRoomKeyIDJob::SetRoomKeyIDJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SetRoomKeyIDJob::~SetRoomKeyIDJob() = default;

bool SetRoomKeyIDJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetRoomKeyIDJob::start");
    submitPostRequest(json());

    return true;
}

void SetRoomKeyIDJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SetRoomKeyIDJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setUserPublicAndPrivateKeysDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SetRoomKeyIDJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

SetRoomKeyIDJob::SetUserPublicAndPrivateKeysInfo SetRoomKeyIDJob::setUserPublicAndPrivateKeysInfo() const
{
    return mSetUserPublicAndPrivateKeysInfo;
}

void SetRoomKeyIDJob::setSetUserPublicAndPrivateKeysInfo(const SetUserPublicAndPrivateKeysInfo &newSetUserPublicAndPrivateKeysInfo)
{
    mSetUserPublicAndPrivateKeysInfo = newSetUserPublicAndPrivateKeysInfo;
}

bool SetRoomKeyIDJob::requireHttpAuthentication() const
{
    return true;
}

bool SetRoomKeyIDJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mSetUserPublicAndPrivateKeysInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetRoomKeyIDJob: mSetUserPublicAndPrivateKeysInfo is invalid";
        return false;
    }
    return true;
}

QNetworkRequest SetRoomKeyIDJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2ESetUserPublicAndPrivateKeys);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SetRoomKeyIDJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["public_key"_L1] = mSetUserPublicAndPrivateKeysInfo.rsaPublicKey;
    jsonObj["private_key"_L1] = mSetUserPublicAndPrivateKeysInfo.rsaPrivateKey;
    jsonObj["force"_L1] = mSetUserPublicAndPrivateKeysInfo.force;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool SetRoomKeyIDJob::SetUserPublicAndPrivateKeysInfo::isValid() const
{
    return !rsaPublicKey.isEmpty() && !rsaPrivateKey.isEmpty();
}

#include "moc_setroomkeyidjob.cpp"
