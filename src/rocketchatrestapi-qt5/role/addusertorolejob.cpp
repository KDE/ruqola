/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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

AddUserToRoleJob::~AddUserToRoleJob() = default;

bool AddUserToRoleJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("AddUserToRoleJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &AddUserToRoleJob::slotAddUsersToRoleDone);
    return true;
}

void AddUserToRoleJob::slotAddUsersToRoleDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("AddUserToRoleJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT addUsersToRoleDone(replyObject);
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
