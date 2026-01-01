/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setuserpublicandprivatekeysjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SetUserPublicAndPrivateKeysJob::SetUserPublicAndPrivateKeysJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SetUserPublicAndPrivateKeysJob::~SetUserPublicAndPrivateKeysJob() = default;

bool SetUserPublicAndPrivateKeysJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetUserPublicAndPrivateKeysJob::start");
    submitPostRequest(json());

    return true;
}

void SetUserPublicAndPrivateKeysJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SetUserPublicAndPrivateKeysJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setUserPublicAndPrivateKeysDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SetUserPublicAndPrivateKeysJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

SetUserPublicAndPrivateKeysJob::SetUserPublicAndPrivateKeysInfo SetUserPublicAndPrivateKeysJob::setUserPublicAndPrivateKeysInfo() const
{
    return mSetUserPublicAndPrivateKeysInfo;
}

void SetUserPublicAndPrivateKeysJob::setSetUserPublicAndPrivateKeysInfo(const SetUserPublicAndPrivateKeysInfo &newSetUserPublicAndPrivateKeysInfo)
{
    mSetUserPublicAndPrivateKeysInfo = newSetUserPublicAndPrivateKeysInfo;
}

bool SetUserPublicAndPrivateKeysJob::requireHttpAuthentication() const
{
    return true;
}

bool SetUserPublicAndPrivateKeysJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mSetUserPublicAndPrivateKeysInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetUserPublicAndPrivateKeysJob: mSetUserPublicAndPrivateKeysInfo is invalid";
        return false;
    }
    return true;
}

QNetworkRequest SetUserPublicAndPrivateKeysJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2ESetUserPublicAndPrivateKeys);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SetUserPublicAndPrivateKeysJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["public_key"_L1] = mSetUserPublicAndPrivateKeysInfo.rsaPublicKey;
    jsonObj["private_key"_L1] = mSetUserPublicAndPrivateKeysInfo.rsaPrivateKey;
    jsonObj["force"_L1] = mSetUserPublicAndPrivateKeysInfo.force;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool SetUserPublicAndPrivateKeysJob::SetUserPublicAndPrivateKeysInfo::isValid() const
{
    return !rsaPublicKey.isEmpty() && !rsaPrivateKey.isEmpty();
}

#include "moc_setuserpublicandprivatekeysjob.cpp"
