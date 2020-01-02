/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "deleteemojicustomjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
DeleteEmojiCustomJob::DeleteEmojiCustomJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

DeleteEmojiCustomJob::~DeleteEmojiCustomJob()
{
}

bool DeleteEmojiCustomJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("DeleteEmojiCustomJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &DeleteEmojiCustomJob::slotDeleteEmojiFinished);
    return true;
}

void DeleteEmojiCustomJob::slotDeleteEmojiFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("DeleteEmojiCustomJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT removeOwnerDone();
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("DeleteEmojiCustomJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString DeleteEmojiCustomJob::emojiId() const
{
    return mEmojiId;
}

void DeleteEmojiCustomJob::setEmojiId(const QString &emojiId)
{
    mEmojiId = emojiId;
}

bool DeleteEmojiCustomJob::requireHttpAuthentication() const
{
    return true;
}

bool DeleteEmojiCustomJob::canStart() const
{
    if (mEmojiId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DeleteEmojiCustomJob: remove mEmojiId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start DeleteEmojiCustomJob job";
        return false;
    }
    return true;
}

QJsonDocument DeleteEmojiCustomJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("emojiId")] = emojiId();
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest DeleteEmojiCustomJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::DeleteEmojiCustom);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
