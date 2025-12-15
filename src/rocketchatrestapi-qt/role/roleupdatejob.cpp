/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleupdatejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
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
    submitPostRequest(json());
    return true;
}

void RoleUpdateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoleUpdateJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT updateRoleDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoleUpdateJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
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
    jsonObj["name"_L1] = mRoleCreateInfo.name;
    jsonObj["description"_L1] = mRoleCreateInfo.description;
    jsonObj["scope"_L1] = mRoleCreateInfo.scope;
    jsonObj["mandatory2fa"_L1] = mRoleCreateInfo.mandatory2fa;
    jsonObj["roleId"_L1] = mRoleCreateInfo.identifier;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_roleupdatejob.cpp"
