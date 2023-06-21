/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolecreatejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

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
    submitPostRequest(json());
    return true;
}

void RoleCreateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RoleCreateJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT createRoleDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RoleCreateJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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

QString RoleCreateJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == QLatin1String("error-action-not-allowed")) {
        return i18n("This is an enterprise feature");
    }

    return RestApiAbstractJob::errorMessage(str, detail);
}

#include "moc_rolecreatejob.cpp"
