/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "roledeletejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &RoleDeleteJob::slotDeleteRoleDone);
    return true;
}

void RoleDeleteJob::slotDeleteRoleDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RoleDeleteJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT deleteRoleDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RoleDeleteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
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
    jsonObj[QLatin1String("roleId")] = mRoleId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
