/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "addkeytochainjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
AddKeyToChainJob::AddKeyToChainJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AddKeyToChainJob::~AddKeyToChainJob()
{
}

bool AddKeyToChainJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("AddKeyToChainJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &AddKeyToChainJob::slotAddKeyToChainFinished);
    return true;
}

void AddKeyToChainJob::slotAddKeyToChainFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        addLoggerInfo(QByteArrayLiteral("AddKeyToChainJob: finished: ") + data);
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Change favorite status done: " << data;
            Q_EMIT addKeyToChainDone();
        } else {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) <<" Problem when we tried to change favorite status" << data;
        }
    }
    deleteLater();
}

QString AddKeyToChainJob::rsaPrivateKey() const
{
    return mRsaPrivateKey;
}

void AddKeyToChainJob::setRsaPrivateKey(const QString &rsaPrivateKey)
{
    mRsaPrivateKey = rsaPrivateKey;
}

QString AddKeyToChainJob::rsaPublicKey() const
{
    return mRsaPublicKey;
}

void AddKeyToChainJob::setRsaPublicKey(const QString &rsaPublicKey)
{
    mRsaPublicKey = rsaPublicKey;
}

bool AddKeyToChainJob::requireHttpAuthentication() const
{
    return true;
}

bool AddKeyToChainJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start AddKeyToChainJob";
        return false;
    }
    if (mRsaPrivateKey.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "AddKeyToChainJob: private key is empty";
        return false;
    }
    if (mRsaPublicKey.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "AddKeyToChainJob: public key is empty";
        return false;
    }
    return true;
}

QNetworkRequest AddKeyToChainJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsFavorite);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}

QJsonDocument AddKeyToChainJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("RSAPubKey")] = mRsaPublicKey;
    jsonObj[QLatin1String("RSAEPrivKey")] = mRsaPrivateKey;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
