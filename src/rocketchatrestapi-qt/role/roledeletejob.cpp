/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roledeletejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RoleDeleteJob::RoleDeleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoleDeleteJob::~RoleDeleteJob() = default;

bool RoleDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoleDeleteJob::start");
    submitPostRequest(json());
    return true;
}

void RoleDeleteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoleDeleteJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT deleteRoleDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoleDeleteJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QString &RoleDeleteJob::roleId() const
{
    return mRoleId;
}

void RoleDeleteJob::setRoleId(const QString &newRoleId)
{
    mRoleId = newRoleId;
}

bool RoleDeleteJob::requireHttpAuthentication() const
{
    return true;
}

bool RoleDeleteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoleId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoleDeleteJob: mRoleId is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest RoleDeleteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RolesDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RoleDeleteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roleId"_L1] = mRoleId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_roledeletejob.cpp"
