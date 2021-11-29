/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomalljob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
EmojiCustomAllJob::EmojiCustomAllJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

EmojiCustomAllJob::~EmojiCustomAllJob() = default;

bool EmojiCustomAllJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start EmojiCustomAllJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("EmojiCustomAllJob: Load Emoji custom"));
    connect(reply, &QNetworkReply::finished, this, &EmojiCustomAllJob::slotEmojiCustomAllDone);

    return true;
}

void EmojiCustomAllJob::slotEmojiCustomAllDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("EmojiCustomAllJob done: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT emojiCustomAllDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("EmojiCustomAllJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool EmojiCustomAllJob::requireHttpAuthentication() const
{
    return true;
}

bool EmojiCustomAllJob::hasQueryParameterSupport() const
{
    return true;
}

QNetworkRequest EmojiCustomAllJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::EmojiCustomAll);

    QUrlQuery queryUrl;
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}
