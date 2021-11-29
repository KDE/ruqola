/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loademojicustomjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
LoadEmojiCustomJob::LoadEmojiCustomJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

LoadEmojiCustomJob::~LoadEmojiCustomJob() = default;

bool LoadEmojiCustomJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start LoadEmojiCustomJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("LoadEmojiCustomJob: Load Emoji custom"));
    connect(reply, &QNetworkReply::finished, this, &LoadEmojiCustomJob::slotloadEmojiCustomDone);

    return true;
}

void LoadEmojiCustomJob::slotloadEmojiCustomDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("LoadEmojiCustomJob done: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT loadEmojiCustomDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("LoadEmojiCustomJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool LoadEmojiCustomJob::requireHttpAuthentication() const
{
    return true;
}

bool LoadEmojiCustomJob::hasQueryParameterSupport() const
{
    // Since 0.71
    return true;
}

QNetworkRequest LoadEmojiCustomJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::LoadEmojiCustom);
    QNetworkRequest req(url);
    QUrlQuery queryUrl;
    addQueryParameter(queryUrl);
    if (!queryUrl.isEmpty()) {
        url.setQuery(queryUrl);
    }
    addAuthRawHeader(req);
    return req;
}
