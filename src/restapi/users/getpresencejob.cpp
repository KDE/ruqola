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

#include "getpresencejob.h"
#include "ruqola_restapi_debug.h"
#include "restapimethod.h"
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>

GetPresenceJob::GetPresenceJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetPresenceJob::~GetPresenceJob()
{
}

bool GetPresenceJob::canStart() const
{
    if (mPresenceUserId.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "userid is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start getPresence job";
        return false;
    }
    return true;
}

bool GetPresenceJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &GetPresenceJob::slotGetPresenceUserId);
    addLoggerInfo("GetPresenceJob ask for presenceUserId: " + mPresenceUserId.toLatin1());

    return true;
}

void GetPresenceJob::slotGetPresenceUserId()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        addLoggerInfo(QByteArrayLiteral("GetPresenceJob: finished: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getPresenceDone(replyObject);
    }
    deleteLater();
}

QString GetPresenceJob::presenceUserId() const
{
    return mPresenceUserId;
}

void GetPresenceJob::setPresenceUserId(const QString &presenceUserId)
{
    mPresenceUserId = presenceUserId;
}

QNetworkRequest GetPresenceJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersGetPresence);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("userId"), mPresenceUserId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    return request;
}

bool GetPresenceJob::requireHttpAuthentication() const
{
    return true;
}

