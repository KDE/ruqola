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

#include "emojicustomcreatejob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
EmojiCustomCreateJob::EmojiCustomCreateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

EmojiCustomCreateJob::~EmojiCustomCreateJob()
{
}

bool EmojiCustomCreateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("EmojiCustomCreateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &EmojiCustomCreateJob::slotEmojiCustomCreateFinished);
    return true;
}

void EmojiCustomCreateJob::slotEmojiCustomCreateFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("EmojiCustomCreateJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT emojiCustomCreateDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("EmojiCustomCreateJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString EmojiCustomCreateJob::emojiId() const
{
    return mEmojiId;
}

void EmojiCustomCreateJob::setEmojiId(const QString &emojiId)
{
    mEmojiId = emojiId;
}

bool EmojiCustomCreateJob::requireHttpAuthentication() const
{
    return true;
}

bool EmojiCustomCreateJob::canStart() const
{
    if (mEmojiId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DeleteEmojiCustomJob: remove mEmojiId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument EmojiCustomCreateJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("emojiId")] = emojiId();
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest EmojiCustomCreateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
