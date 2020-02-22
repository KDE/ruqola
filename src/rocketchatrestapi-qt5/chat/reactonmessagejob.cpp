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

#include "reactonmessagejob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ReactOnMessageJob::ReactOnMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ReactOnMessageJob::~ReactOnMessageJob()
{
}

bool ReactOnMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ReactOnMessageJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ReactOnMessageJob::slotReactonMessageFinished);
    return true;
}

void ReactOnMessageJob::slotReactonMessageFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ReactOnMessageJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT reactOnMessageDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ReactOnMessageJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString ReactOnMessageJob::emoji() const
{
    return mEmoji;
}

void ReactOnMessageJob::setEmoji(const QString &emoji)
{
    mEmoji = emoji;
}

void ReactOnMessageJob::setShouldReact(bool added)
{
    mAddReact = added;
}

bool ReactOnMessageJob::shouldReact() const
{
    return mAddReact;
}

bool ReactOnMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool ReactOnMessageJob::canStart() const
{
    if (mEmoji.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ReactOnMessageJob: emoji is empty";
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ReactOnMessageJob: messageId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ReactOnMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("emoji")] = mEmoji;
    jsonObj[QLatin1String("messageId")] = mMessageId;
    jsonObj[QLatin1String("shouldReact")] = mAddReact;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ReactOnMessageJob::messageId() const
{
    return mMessageId;
}

void ReactOnMessageJob::setMessageId(const QString &t)
{
    mMessageId = t;
}

QNetworkRequest ReactOnMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatReact);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
