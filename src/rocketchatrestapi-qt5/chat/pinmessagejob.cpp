/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "pinmessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
PinMessageJob::PinMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

PinMessageJob::~PinMessageJob()
{
}

bool PinMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("StarMessageJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &PinMessageJob::slotPinMessage);
    return true;
}

void PinMessageJob::slotPinMessage()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        addLoggerInfo(QByteArrayLiteral("PinMessageJob: success: ") + data);
        if (mPinMessage) {
            Q_EMIT pinMessageDone();
        } else {
            Q_EMIT unPinMessageDone();
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool PinMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool PinMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start PinMessageJob";
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "PinMessageJob: mMessageId is empty";
        return false;
    }
    return true;
}

QNetworkRequest PinMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(mPinMessage ? RestApiUtil::RestApiUrlType::ChatPinMessage : RestApiUtil::RestApiUrlType::ChatUnPinMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QString PinMessageJob::messageId() const
{
    return mMessageId;
}

void PinMessageJob::setMessageId(const QString &messageId)
{
    mMessageId = messageId;
}

bool PinMessageJob::pinMessage() const
{
    return mPinMessage;
}

void PinMessageJob::setPinMessage(bool pinMessage)
{
    mPinMessage = pinMessage;
}

QJsonDocument PinMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("messageId")] = mMessageId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
