/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "usersinfojob.h"
#include "restapimethod.h"
#include "restapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QNetworkReply>

UsersInfoJob::UsersInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersInfoJob::~UsersInfoJob()
{
}

bool UsersInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersInfoJob::start()
{
    if (!canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start usersinfo job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &UsersInfoJob::slotOwnInfoFinished);
    addLoggerInfo(QByteArrayLiteral("UsersInfoJob: Ask info about me"));
    return true;
}

void UsersInfoJob::slotOwnInfoFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        addLoggerInfo(QByteArrayLiteral("UsersInfoJob: finished: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT usersInfoDone(replyObject);
    }
    deleteLater();
}

bool UsersInfoJob::useUserName() const
{
    return mUseUserName;
}

void UsersInfoJob::setUseUserName(bool useUserName)
{
    mUseUserName = useUserName;
}

QString UsersInfoJob::identifier() const
{
    return mIdentifier;
}

void UsersInfoJob::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

QNetworkRequest UsersInfoJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersInfo);
    QUrlQuery queryUrl;
    if (mUseUserName) {
        queryUrl.addQueryItem(QStringLiteral("username"), mIdentifier);
    } else {
        queryUrl.addQueryItem(QStringLiteral("userId"), mIdentifier);
    }
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    return request;
}

bool UsersInfoJob::canStart() const
{
    if (mIdentifier.trimmed().isEmpty()) {
        qCWarning(RESTAPI_LOG) << "UsersInfoJob: identifier is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start SpotlightJob job";
        return false;
    }
    return true;
}
