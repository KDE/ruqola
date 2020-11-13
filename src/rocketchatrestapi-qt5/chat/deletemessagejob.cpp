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

#include "deletemessagejob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
DeleteMessageJob::DeleteMessageJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

DeleteMessageJob::~DeleteMessageJob()
{
}

bool DeleteMessageJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("DeleteMessageJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &DeleteMessageJob::slotDeleteMessageFinished);
    return true;
}

void DeleteMessageJob::slotDeleteMessageFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("DeleteMessageJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT deleteMessageDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("DeleteMessageJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool DeleteMessageJob::requireHttpAuthentication() const
{
    return true;
}

bool DeleteMessageJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DeleteMessageJob: roomId is empty";
        return false;
    }
    if (mMessageId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DeleteMessageJob: messageId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument DeleteMessageJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("msgId")] = messageId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString DeleteMessageJob::roomId() const
{
    return mRoomId;
}

void DeleteMessageJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString DeleteMessageJob::messageId() const
{
    return mMessageId;
}

void DeleteMessageJob::setMessageId(const QString &t)
{
    mMessageId = t;
}

QNetworkRequest DeleteMessageJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChatDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QString DeleteMessageJob::jobName() const
{
    return QStringLiteral("Delete Message");
}

QString DeleteMessageJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1String("error-action-not-allowed")) {
        return i18n("Message deletion is not allowed.");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}
