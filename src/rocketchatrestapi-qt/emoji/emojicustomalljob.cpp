/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomalljob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
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
    submitGetRequest();
    addStartRestApiInfo("EmojiCustomAllJob: Load Emoji custom"_ba);

    return true;
}

void EmojiCustomAllJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("EmojiCustomAllJob done: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT emojiCustomAllDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("EmojiCustomAllJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
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

#include "moc_emojicustomalljob.cpp"
