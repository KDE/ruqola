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

#include "permissionupdatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
PermissionUpdateJob::PermissionUpdateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

PermissionUpdateJob::~PermissionUpdateJob() = default;

bool PermissionUpdateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("PermissionUpdateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &PermissionUpdateJob::slotPermissionUpdateFinished);
    return true;
}

void PermissionUpdateJob::slotPermissionUpdateFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("PermissionUpdateJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT permissionUpdateDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("PermissionUpdateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

const QMap<QString, QStringList> &PermissionUpdateJob::permissions() const
{
    return mPermissions;
}

void PermissionUpdateJob::setPermissions(const QMap<QString, QStringList> &newPermissions)
{
    mPermissions = newPermissions;
}

bool PermissionUpdateJob::requireHttpAuthentication() const
{
    return true;
}

bool PermissionUpdateJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mPermissions.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "PermissionUpdateJob: mPermissions is empty";
        return false;
    }
    return true;
}

QNetworkRequest PermissionUpdateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::PermissionsUpdate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument PermissionUpdateJob::json() const
{
    QJsonObject jsonObj;
    QJsonArray array;
    QMapIterator<QString, QStringList> i(mPermissions);
    while (i.hasNext()) {
        i.next();
        QJsonObject obj;
        obj[QLatin1String("_id")] = i.key();
        obj[QLatin1String("roles")] = QJsonArray::fromStringList(i.value());
        array.append(obj);
    }
    jsonObj[QLatin1String("permissions")] = array;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
