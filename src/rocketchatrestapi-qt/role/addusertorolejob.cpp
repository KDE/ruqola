/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addusertorolejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
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
    submitPostRequest(json());
    return true;
}

void AddUserToRoleJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("AddUserToRoleJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT addUsersToRoleDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("AddUserToRoleJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool AddUserToRoleJob::useRC80() const
{
    return mUseRC80;
}

void AddUserToRoleJob::setUseRC80(bool newUseRC80)
{
    mUseRC80 = newUseRC80;
}

const QString &AddUserToRoleJob::roleId() const
{
    return mRoleId;
}

void AddUserToRoleJob::setRoleId(const QString &newRoleId)
{
    mRoleId = newRoleId;
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
    if (mUseRC80) {
        if (mRoleId.isEmpty()) {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << "AddUserToRoleJob: mRoleId is empty.";
            return false;
        }
    } else {
        if (mRoleName.isEmpty()) {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << "AddUserToRoleJob: mRoleName is empty.";
            return false;
        }
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
    jsonObj["username"_L1] = mUsername;
    if (mUseRC80) {
        jsonObj["roleId"_L1] = mRoleId;
    } else {
        jsonObj["roleName"_L1] = mRoleName;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_addusertorolejob.cpp"
