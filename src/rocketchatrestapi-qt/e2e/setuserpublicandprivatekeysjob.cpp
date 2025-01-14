/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setuserpublicandprivatekeysjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
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
        addLoggerInfo(QByteArrayLiteral("SetUserPublicAndPrivateKeysJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT addKeyToChainDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SetUserPublicAndPrivateKeysJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString SetUserPublicAndPrivateKeysJob::rsaPrivateKey() const
{
    return mRsaPrivateKey;
}

void SetUserPublicAndPrivateKeysJob::setRsaPrivateKey(const QString &rsaPrivateKey)
{
    mRsaPrivateKey = rsaPrivateKey;
}

QString SetUserPublicAndPrivateKeysJob::rsaPublicKey() const
{
    return mRsaPublicKey;
}

void SetUserPublicAndPrivateKeysJob::setRsaPublicKey(const QString &rsaPublicKey)
{
    mRsaPublicKey = rsaPublicKey;
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
    if (mRsaPrivateKey.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetUserPublicAndPrivateKeysJob: private key is empty";
        return false;
    }
    if (mRsaPublicKey.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetUserPublicAndPrivateKeysJob: public key is empty";
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
    jsonObj["public_key"_L1] = mRsaPublicKey;
    jsonObj["private_key"_L1] = mRsaPrivateKey;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_setuserpublicandprivatekeysjob.cpp"
