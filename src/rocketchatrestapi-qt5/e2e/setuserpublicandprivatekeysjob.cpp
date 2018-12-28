/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "setuserpublicandprivatekeysjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SetUserPublicAndPrivateKeysJob::SetUserPublicAndPrivateKeysJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SetUserPublicAndPrivateKeysJob::~SetUserPublicAndPrivateKeysJob()
{
}

bool SetUserPublicAndPrivateKeysJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("SetUserPublicAndPrivateKeysJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &SetUserPublicAndPrivateKeysJob::slotAddKeyToChainFinished);
    return true;
}

void SetUserPublicAndPrivateKeysJob::slotAddKeyToChainFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SetUserPublicAndPrivateKeysJob: sucess: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT addKeyToChainDone();
        } else {
            addLoggerWarning(QByteArrayLiteral("SetUserPublicAndPrivateKeysJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
    }
    deleteLater();
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
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SetUserPublicAndPrivateKeysJob";
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
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}

QJsonDocument SetUserPublicAndPrivateKeysJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("public_key")] = mRsaPublicKey;
    jsonObj[QLatin1String("private_key")] = mRsaPrivateKey;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
