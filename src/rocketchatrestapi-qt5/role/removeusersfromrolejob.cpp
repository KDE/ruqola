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

#include "removeusersfromrolejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
RemoveUsersFromRoleJob::RemoveUsersFromRoleJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RemoveUsersFromRoleJob::~RemoveUsersFromRoleJob()
{
}

bool RemoveUsersFromRoleJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RemoveUsersFromRoleJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &RemoveUsersFromRoleJob::slotRemoveUsersFromRoleDone);
    return true;
}

void RemoveUsersFromRoleJob::slotRemoveUsersFromRoleDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RemoveUsersFromRoleJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT removeUsersFromRoleDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RemoveUsersFromRoleJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

const QString &RemoveUsersFromRoleJob::username() const
{
    return mUsername;
}

void RemoveUsersFromRoleJob::setUsername(const QString &newUsername)
{
    mUsername = newUsername;
}

const QString &RemoveUsersFromRoleJob::roleName() const
{
    return mRoleName;
}

void RemoveUsersFromRoleJob::setRoleName(const QString &newRoleName)
{
    mRoleName = newRoleName;
}

bool RemoveUsersFromRoleJob::requireHttpAuthentication() const
{
    return true;
}

bool RemoveUsersFromRoleJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoleName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RemoveUsersFromRoleJob: mRoleName is not valid.";
        return false;
    }
    if (mUsername.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RemoveUsersFromRoleJob: mUsername is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest RemoveUsersFromRoleJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RolesRemoveUsersFromRole);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RemoveUsersFromRoleJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roleName")] = mRoleName;
    jsonObj[QLatin1String("username")] = mUsername;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
