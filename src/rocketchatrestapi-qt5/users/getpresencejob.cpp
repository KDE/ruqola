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

#include "getpresencejob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
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
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "presenceuserid is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
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
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetPresenceJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT getPresenceDone(replyObject[QStringLiteral("presence")].toString());
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("GetPresenceJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
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
    addAuthRawHeader(request);
    return request;
}

bool GetPresenceJob::requireHttpAuthentication() const
{
    return true;
}
