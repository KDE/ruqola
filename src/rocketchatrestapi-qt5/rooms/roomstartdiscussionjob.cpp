/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "roomstartdiscussionjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
RoomStartdiscussionJob::RoomStartdiscussionJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomStartdiscussionJob::~RoomStartdiscussionJob()
{
}

bool RoomStartdiscussionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("RoomStartdiscussionJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &RoomStartdiscussionJob::slotChangeFavoriteFinished);
    return true;
}

void RoomStartdiscussionJob::slotChangeFavoriteFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RoomStartdiscussionJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT changeFavoriteDone();
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("RoomStartdiscussionJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString RoomStartdiscussionJob::parentMessageId() const
{
    return mParentMessageId;
}

void RoomStartdiscussionJob::setParentMessageId(const QString &parentMessageId)
{
    mParentMessageId = parentMessageId;
}

QString RoomStartdiscussionJob::discussionName() const
{
    return mDiscussionName;
}

void RoomStartdiscussionJob::setDiscussionName(const QString &discussionName)
{
    mDiscussionName = discussionName;
}

QString RoomStartdiscussionJob::parentId() const
{
    return mParentId;
}

void RoomStartdiscussionJob::setParentId(const QString &parentId)
{
    mParentId = parentId;
}

bool RoomStartdiscussionJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomStartdiscussionJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RoomStartdiscussionJob";
        return false;
    }
    if (mParentId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomStartdiscussionJob: mParentId is empty";
        return false;
    }
    if (mDiscussionName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomStartdiscussionJob: mDiscussionName is empty";
        return false;
    }
    return true;
}

QNetworkRequest RoomStartdiscussionJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsFavorite);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RoomStartdiscussionJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("prid")] = mParentId;
    jsonObj[QLatin1String("t_name")] = mDiscussionName;
    if (!mParentMessageId.isEmpty()) {
        jsonObj[QLatin1String("pmid")] = mParentMessageId;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
