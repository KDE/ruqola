/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "usersautocompletejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UsersAutocompleteJob::UsersAutocompleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersAutocompleteJob::~UsersAutocompleteJob()
{
}

bool UsersAutocompleteJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersAutocompleteJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start UsersAutocompleteJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &UsersAutocompleteJob::slotUsersAutocompleteFinished);
    addLoggerInfo(QByteArrayLiteral("UsersAutocompleteJob: Ask info about me"));
    return true;
}

void UsersAutocompleteJob::slotUsersAutocompleteFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("UsersAutocompleteJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT usersAutocompleteDone(replyObject);
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("UsersAutocompleteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest UsersAutocompleteJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersAutocomplete);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("selector"), mSelector);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    return request;
}

QString UsersAutocompleteJob::selector() const
{
    return mSelector;
}

void UsersAutocompleteJob::setSelector(const QString &selector)
{
    mSelector = selector;
}

bool UsersAutocompleteJob::canStart() const
{
    if (mSelector.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersAutocompleteJob: selector is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start UsersAutocompleteJob job";
        return false;
    }
    return true;
}
