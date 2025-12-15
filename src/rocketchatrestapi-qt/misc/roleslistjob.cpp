/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleslistjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
RolesListJob::RolesListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RolesListJob::~RolesListJob() = default;

bool RolesListJob::requireHttpAuthentication() const
{
    return true;
}

bool RolesListJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RolesListJob";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("RolesListJob: Ask for roles"_ba);
    return true;
}

void RolesListJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RolesListJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT rolesListDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RolesListJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest RolesListJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RolesList);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_roleslistjob.cpp"
