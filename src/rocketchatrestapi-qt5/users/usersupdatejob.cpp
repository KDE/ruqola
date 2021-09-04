/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "usersupdatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
UsersUpdateJob::UsersUpdateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersUpdateJob::~UsersUpdateJob()
{
}

bool UsersUpdateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersUpdateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &UsersUpdateJob::slotUsersUpdate);
    return true;
}

void UsersUpdateJob::slotUsersUpdate()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("UsersUpdateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT usersUpdateDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("UsersUpdateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

UpdateUserInfo UsersUpdateJob::updateInfo() const
{
    return mUpdateInfo;
}

void UsersUpdateJob::setUpdateInfo(const UpdateUserInfo &updateInfo)
{
    mUpdateInfo = updateInfo;
}

QString UsersUpdateJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == QLatin1String("totp-required")) {
        return i18n("Authentication required");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

bool UsersUpdateJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersUpdateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mUpdateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UsersUpdateJob: mUpdateInfo is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest UsersUpdateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersUpdateJob::json() const
{
    return mUpdateInfo.json();
}

