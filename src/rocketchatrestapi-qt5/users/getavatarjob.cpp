/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "getavatarjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
GetAvatarJob::GetAvatarJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetAvatarJob::~GetAvatarJob()
{
}

bool GetAvatarJob::canStart() const
{
    if (mAvatarUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Avatar userid is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start getavatar job";
        return false;
    }
    return true;
}

bool GetAvatarJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &GetAvatarJob::slotGetAvatarInfo);
    addLoggerInfo("GetAvatarJob ask for avatarUserId: " + mAvatarUserId.toLatin1());
    reply->setProperty("userId", mAvatarUserId);

    return true;
}

void GetAvatarJob::slotGetAvatarInfo()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject.contains(QStringLiteral("success"))) {
            if (!replyObject[QStringLiteral("success")].toBool()) {
                addLoggerWarning(QByteArrayLiteral("GetAvatarJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
                qWarning() << "void GetAvatarJob::slotGetAvatarInfo() ERROR :" << reply->property("userId").toString();
            } else {
                addLoggerInfo(QByteArrayLiteral("GetAvatarJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
                qCWarning(ROCKETCHATQTRESTAPI_LOG) << " not implemented ! API changed !";
            }
            Q_EMIT redownloadAvatar();
        } else {
            QString str = QString::fromUtf8(data);
            str.remove(QLatin1Char('"'));
            const QString userId = reply->property("userId").toString();
            addLoggerWarning(QByteArrayLiteral("GetAvatarJob success: ") + data);
            Q_EMIT avatar(userId, str);
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest GetAvatarJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersGetAvatar);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("userId"), mAvatarUserId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    return request;
}

bool GetAvatarJob::requireHttpAuthentication() const
{
    return false;
}

QString GetAvatarJob::avatarUserId() const
{
    return mAvatarUserId;
}

void GetAvatarJob::setAvatarUserId(const QString &avatarUserId)
{
    mAvatarUserId = avatarUserId;
}


QString RocketChatRestApi::GetAvatarJob::jobName() const
{
    return i18n("Get Avatar");
}
