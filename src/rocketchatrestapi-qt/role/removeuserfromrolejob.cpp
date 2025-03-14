/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeuserfromrolejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RemoveUserFromRoleJob::RemoveUserFromRoleJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RemoveUserFromRoleJob::~RemoveUserFromRoleJob() = default;

bool RemoveUserFromRoleJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RemoveUsersFromRoleJob::start");
    submitPostRequest(json());
    return true;
}

void RemoveUserFromRoleJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RemoveUsersFromRoleJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT removeUsersFromRoleDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RemoveUsersFromRoleJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString RemoveUserFromRoleJob::errorMessage(const QString &str, const QJsonObject &details)
{
    if (str == "error-user-not-in-role"_L1) {
        return i18n("This user is not in this role.");
    }
    return RestApiAbstractJob::errorMessage(str, details);
}

const QString &RemoveUserFromRoleJob::username() const
{
    return mUsername;
}

void RemoveUserFromRoleJob::setUsername(const QString &newUsername)
{
    mUsername = newUsername;
}

const QString &RemoveUserFromRoleJob::roleName() const
{
    return mRoleName;
}

void RemoveUserFromRoleJob::setRoleName(const QString &newRoleName)
{
    mRoleName = newRoleName;
}

bool RemoveUserFromRoleJob::requireHttpAuthentication() const
{
    return true;
}

bool RemoveUserFromRoleJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoleName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RemoveUsersFromRoleJob: mRoleName is not valid.";
        return false;
    }
    if (mUsername.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RemoveUsersFromRoleJob: mUsername is not valid.";
        return false;
    }
    return true;
}

QNetworkRequest RemoveUserFromRoleJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RolesRemoveUserFromRole);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RemoveUserFromRoleJob::json() const
{
    // TODO restapi seems to not working. Need to report bug.
    QJsonObject jsonObj;
    jsonObj["roleName"_L1] = mRoleName;
    jsonObj["username"_L1] = mUsername;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_removeuserfromrolejob.cpp"
