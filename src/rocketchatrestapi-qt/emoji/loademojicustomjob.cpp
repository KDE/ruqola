/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

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
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("LoadEmojiCustomJob: Load Emoji custom"));

    return true;
}

void LoadEmojiCustomJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("LoadEmojiCustomJob done: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT loadEmojiCustomDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("LoadEmojiCustomJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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

#include "moc_loademojicustomjob.cpp"
