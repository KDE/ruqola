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

#include "addusertorolejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
AddUserToRoleJob::AddUserToRoleJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AddUserToRoleJob::~AddUserToRoleJob()
{
}

bool AddUserToRoleJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("AddUserToRoleJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &AddUserToRoleJob::slotRemoveUsersFromRoleDone);
    return true;
}

void AddUserToRoleJob::slotRemoveUsersFromRoleDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("AddUserToRoleJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT removeUsersFromRoleDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("AddUserToRoleJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

const QString &AddUserToRoleJob::username() const
{
    return mUsername;
}

void AddUserToRoleJob::setUsername(const QString &newUsername)
{
    mUsername = newUsername;
}

const QString &AddUserToRoleJob::roleName() const
{
    return mRoleName;
}

void AddUserToRoleJob::setRoleName(const QString &newRoleName)
{
    mRoleName = newRoleName;
}

bool AddUserToRoleJob::requireHttpAuthentication() const
{
    return true;
}

bool AddUserToRoleJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoleName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "AddUserToRoleJob: mRoleName is not valid.";
        return false;
    }
    if (mUsername.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "AddUserToRoleJob: mUsername is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest AddUserToRoleJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RolesAddUserToRole);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument AddUserToRoleJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roleName")] = mRoleName;
    jsonObj[QLatin1String("username")] = mUsername;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
