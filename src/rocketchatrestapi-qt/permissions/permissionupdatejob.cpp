/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    submitPostRequest(json());

    return true;
}

void PermissionUpdateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("PermissionUpdateJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT permissionUpdateDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("PermissionUpdateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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
        array.append(std::move(obj));
    }
    jsonObj[QLatin1String("permissions")] = array;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_permissionupdatejob.cpp"
