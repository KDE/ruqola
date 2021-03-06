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

#include "setavatarjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SetAvatarJob::SetAvatarJob(QObject *parent)
    : UserBaseJob(parent)
{
}

SetAvatarJob::~SetAvatarJob()
{
}

bool SetAvatarJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetAvatarJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &SetAvatarJob::slotSetAvatar);
    return true;
}

void SetAvatarJob::slotSetAvatar()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SetAvatarJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT setAvatarDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("SetAvatarJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

SetAvatarJob::SetAvatarInfo SetAvatarJob::avatarInfo() const
{
    return mAvatarInfo;
}

void SetAvatarJob::setAvatarInfo(const SetAvatarInfo &avatarInfo)
{
    mAvatarInfo = avatarInfo;
}

QString SetAvatarJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1String("error-avatar-invalid-url")) {
        const QString url = details[QLatin1String("url")].toString();
        return i18n("Invalid avatar URL: %1", url);
    } else if (str == QLatin1String("error-avatar-url-handling")) {
        const QString url = details[QLatin1String("url")].toString();
        const QString username = details[QLatin1String("username")].toString();
        return i18n("Error while handling avatar setting from a URL \"%1\" for %2", url, username);
    }

    return RestApiAbstractJob::errorMessage(str, details);
}

bool SetAvatarJob::requireHttpAuthentication() const
{
    return true;
}

bool SetAvatarJob::canStart() const
{
    if (!hasUserIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetAvatarJob: identifier is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mAvatarInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetAvatarJob: mAvatarInfo is not valid";
        return false;
    }
    return true;
}

QNetworkRequest SetAvatarJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersSetAvatar);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SetAvatarJob::json() const
{
    QJsonObject jsonObj;
    if (!mAvatarInfo.mAvatarUrl.isEmpty()) {
        jsonObj[QLatin1String("avatarUrl")] = mAvatarInfo.mAvatarUrl;
    } else if (!mAvatarInfo.mAvatarUserStr.isEmpty()) {
        // USe multipart here.
    }
    generateJson(jsonObj);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool SetAvatarJob::SetAvatarInfo::isValid() const
{
    return !mAvatarUrl.isEmpty() && !mAvatarUserStr.isEmpty();
}
