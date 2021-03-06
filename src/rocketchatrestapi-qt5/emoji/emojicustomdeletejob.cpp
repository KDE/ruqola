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

#include "emojicustomdeletejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
EmojiCustomDeleteJob::EmojiCustomDeleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

EmojiCustomDeleteJob::~EmojiCustomDeleteJob()
{
}

bool EmojiCustomDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("DeleteEmojiCustomJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &EmojiCustomDeleteJob::slotDeleteEmojiFinished);
    return true;
}

void EmojiCustomDeleteJob::slotDeleteEmojiFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("DeleteEmojiCustomJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT emojiCustomDeleteDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("DeleteEmojiCustomJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString EmojiCustomDeleteJob::emojiId() const
{
    return mEmojiId;
}

void EmojiCustomDeleteJob::setEmojiId(const QString &emojiId)
{
    mEmojiId = emojiId;
}

bool EmojiCustomDeleteJob::requireHttpAuthentication() const
{
    return true;
}

bool EmojiCustomDeleteJob::canStart() const
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

QJsonDocument EmojiCustomDeleteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("emojiId")] = emojiId();
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest EmojiCustomDeleteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
