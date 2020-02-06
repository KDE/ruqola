/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "getusernamesuggestionjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
GetUsernameSuggestionJob::GetUsernameSuggestionJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetUsernameSuggestionJob::~GetUsernameSuggestionJob()
{
}

bool GetUsernameSuggestionJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getPresence job";
        return false;
    }
    return true;
}

bool GetUsernameSuggestionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &GetUsernameSuggestionJob::slotGetUsernameSuggestion);
    addLoggerInfo("GetUsernameSuggestionJob start");

    return true;
}

void GetUsernameSuggestionJob::slotGetUsernameSuggestion()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetUsernameSuggestionJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT getUsernameSuggestionDone(replyObject[QStringLiteral("result")].toString());
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("GetUsernameSuggestionJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest GetUsernameSuggestionJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersGetUsernameSuggestion);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool GetUsernameSuggestionJob::requireHttpAuthentication() const
{
    return true;
}
