/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

LoadEmojiCustomJob::~LoadEmojiCustomJob()
{
}

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
