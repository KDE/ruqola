/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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
