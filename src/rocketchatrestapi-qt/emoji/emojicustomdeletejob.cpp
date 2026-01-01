/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomdeletejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
EmojiCustomDeleteJob::EmojiCustomDeleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

EmojiCustomDeleteJob::~EmojiCustomDeleteJob() = default;

bool EmojiCustomDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("DeleteEmojiCustomJob::start");
    submitPostRequest(json());

    return true;
}

void EmojiCustomDeleteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("DeleteEmojiCustomJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT emojiCustomDeleteDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("DeleteEmojiCustomJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray EmojiCustomDeleteJob::emojiId() const
{
    return mEmojiId;
}

void EmojiCustomDeleteJob::setEmojiId(const QByteArray &emojiId)
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
    jsonObj["emojiId"_L1] = QLatin1StringView(emojiId());
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

#include "moc_emojicustomdeletejob.cpp"
