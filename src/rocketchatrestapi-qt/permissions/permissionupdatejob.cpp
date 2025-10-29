/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionupdatejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
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
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("PermissionUpdateJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT permissionUpdateDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("PermissionUpdateJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    for (const auto &[key, value] : mPermissions.asKeyValueRange()) {
        QJsonObject obj;
        obj["_id"_L1] = key;
        obj["roles"_L1] = QJsonArray::fromStringList(value);
        array.append(std::move(obj));
    }
    jsonObj["permissions"_L1] = array;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_permissionupdatejob.cpp"
