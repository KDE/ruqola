/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serviceconfigurationsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ServiceConfigurationsJob::ServiceConfigurationsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ServiceConfigurationsJob::~ServiceConfigurationsJob() = default;

bool ServiceConfigurationsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ServiceConfigurationsJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo("ServiceConfigurationsJob: Ask public info"_ba);

    return true;
}

void ServiceConfigurationsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("ServiceConfigurationsJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT serviceConfigurationsDone(*replyObject);
    }
}

bool ServiceConfigurationsJob::requireHttpAuthentication() const
{
    return false;
}

QNetworkRequest ServiceConfigurationsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ServiceConfigurations);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

#include "moc_serviceconfigurationsjob.cpp"
