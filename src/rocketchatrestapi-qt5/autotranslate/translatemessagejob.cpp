/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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

#include "translatemessagejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
TranslateMessageJob::TranslateMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TranslateMessageJob::~TranslateMessageJob()
{
}

bool TranslateMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &TranslateMessageJob::slotTranslateMessageFinished);
    return true;
}

void TranslateMessageJob::slotTranslateMessageFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("TranslateMessageJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT translateMessageDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("TranslateMessageJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString TranslateMessageJob::targetLanguage() const
{
    return mTargetLanguage;
}

void TranslateMessageJob::setTargetLanguage(const QString &targetLanguage)
{
    mTargetLanguage = targetLanguage;
}

QString TranslateMessageJob::messageId() const
{
    return mMessageId;
}

void TranslateMessageJob::setMessageId(const QString &messageId)
{
    mMessageId = messageId;
}

bool TranslateMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool TranslateMessageJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TranslateMessageJob: mMessageId is empty";
        return false;
    }
    if (mTargetLanguage.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TranslateMessageJob: mTargetLanguage is empty";
        return false;
    }
    return true;
}

QNetworkRequest TranslateMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::AutoTranslateTranslateMessage);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TranslateMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("messageId")] = mMessageId;
    jsonObj[QLatin1String("targetLanguage")] = mTargetLanguage;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
