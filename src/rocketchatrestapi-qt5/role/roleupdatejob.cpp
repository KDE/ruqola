/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleupdatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
RoleUpdateJob::RoleUpdateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoleUpdateJob::~RoleUpdateJob() = default;

bool RoleUpdateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoleUpdateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &RoleUpdateJob::slotUpdateRoleDone);
    return true;
}

void RoleUpdateJob::slotUpdateRoleDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RoleUpdateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT updateRoleDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RoleUpdateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

RoleUpdateJob::RoleUpdateInfo RoleUpdateJob::updateRoleInfo() const
{
    return mRoleCreateInfo;
}

void RoleUpdateJob::setUpdateRoleInfo(const RoleUpdateInfo &statusCreateInfo)
{
    mRoleCreateInfo = statusCreateInfo;
}

bool RoleUpdateJob::requireHttpAuthentication() const
{
    return true;
}

bool RoleUpdateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mRoleCreateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoleUpdateJob: mRoleCreateInfo is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest RoleUpdateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RolesUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RoleUpdateJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("name")] = mRoleCreateInfo.name;
    jsonObj[QLatin1String("description")] = mRoleCreateInfo.description;
    jsonObj[QLatin1String("scope")] = mRoleCreateInfo.scope;
    jsonObj[QLatin1String("mandatory2fa")] = mRoleCreateInfo.mandatory2fa;
    jsonObj[QLatin1String("roleId")] = mRoleCreateInfo.identifier;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
