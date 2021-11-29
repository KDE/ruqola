/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "emojicustomupdatejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
EmojiCustomUpdateJob::EmojiCustomUpdateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

EmojiCustomUpdateJob::~EmojiCustomUpdateJob() = default;

bool EmojiCustomUpdateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("EmojiCustomUpdateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &EmojiCustomUpdateJob::slotEmojiCustomUpdateFinished);
    return true;
}

void EmojiCustomUpdateJob::slotEmojiCustomUpdateFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("EmojiCustomUpdateJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT emojiCustomUpdateDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("EmojiCustomUpdateJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

const EmojiCustomUpdateJob::EmojiInfo &EmojiCustomUpdateJob::emojiInfo() const
{
    return mEmojiInfo;
}

void EmojiCustomUpdateJob::setEmojiInfo(const EmojiInfo &newEmojiInfo)
{
    mEmojiInfo = newEmojiInfo;
}

bool EmojiCustomUpdateJob::requireHttpAuthentication() const
{
    return true;
}

bool EmojiCustomUpdateJob::canStart() const
{
    if (!mEmojiInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "EmojiCustomUpdateJob: mEmojiInfo is not valid";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument EmojiCustomUpdateJob::json() const
{
    QJsonObject jsonObj;
    // TODO
    // jsonObj[QLatin1String("emojiId")] = emojiId();
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest EmojiCustomUpdateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool EmojiCustomUpdateJob::EmojiInfo::isValid() const
{
    // Alias is optional
    return !name.isEmpty() && !fileNameUrl.isEmpty() && !emojiId.isEmpty();
}
