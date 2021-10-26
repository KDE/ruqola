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

RoleDeleteJob::~RoleDeleteJob()
{
}

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
