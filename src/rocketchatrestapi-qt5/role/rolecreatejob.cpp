/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolecreatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
RoleCreateJob::RoleCreateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoleCreateJob::~RoleCreateJob() = default;

bool RoleCreateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoleCreateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &RoleCreateJob::slotCreateRoleDone);
    return true;
}

void RoleCreateJob::slotCreateRoleDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RoleCreateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT createRoleDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RoleCreateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

RoleCreateJob::RoleCreateInfo RoleCreateJob::createRoleInfo() const
{
    return mRoleCreateInfo;
}

void RoleCreateJob::setCreateRoleInfo(const RoleCreateInfo &statusCreateInfo)
{
    mRoleCreateInfo = statusCreateInfo;
}

bool RoleCreateJob::requireHttpAuthentication() const
{
    return true;
}

bool RoleCreateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mRoleCreateInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoleCreateJob: mRoleCreateInfo is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest RoleCreateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RolesCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RoleCreateJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("name")] = mRoleCreateInfo.name;
    jsonObj[QLatin1String("description")] = mRoleCreateInfo.description;
    jsonObj[QLatin1String("scope")] = mRoleCreateInfo.scope;
    jsonObj[QLatin1String("mandatory2fa")] = mRoleCreateInfo.mandatory2fa;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
