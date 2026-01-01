/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customuserstatuslistjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
CustomUserStatusListJob::CustomUserStatusListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CustomUserStatusListJob::~CustomUserStatusListJob() = default;

bool CustomUserStatusListJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start CustomUserStatusJobList job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("CustomUserStatusJob: Ask custom user status info"_ba);

    return true;
}

void CustomUserStatusListJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("CustomUserStatusJob done: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT customUserStatusDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("CustomUserStatusJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool CustomUserStatusListJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest CustomUserStatusListJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::CustomUserStatusList);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

bool CustomUserStatusListJob::hasQueryParameterSupport() const
{
    return false;
}

#include "moc_customuserstatuslistjob.cpp"
