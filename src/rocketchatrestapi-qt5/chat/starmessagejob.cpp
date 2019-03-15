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

#include "starmessagejob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include "restapirequest.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
StarMessageJob::StarMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

StarMessageJob::~StarMessageJob()
{
}

bool StarMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start star message job";
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "MessageId is empty";
        return false;
    }
    return true;
}

bool StarMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("StarMessageJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &StarMessageJob::slotStarMessageFinished);
    return true;
}

void StarMessageJob::slotStarMessageFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("StarMessageJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT messageStarred();
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("StarMessageJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool StarMessageJob::starMessage() const
{
    return mStarMessage;
}

void StarMessageJob::setStarMessage(bool starMessage)
{
    mStarMessage = starMessage;
}

QJsonDocument StarMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("messageId")] = mMessageId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool StarMessageJob::requireHttpAuthentication() const
{
    return true;
}

QString StarMessageJob::messageId() const
{
    return mMessageId;
}

void StarMessageJob::setMessageId(const QString &messageId)
{
    mMessageId = messageId;
}

QNetworkRequest StarMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(mStarMessage ? RestApiUtil::RestApiUrlType::ChatStarMessage : RestApiUtil::RestApiUrlType::ChatUnStarMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
