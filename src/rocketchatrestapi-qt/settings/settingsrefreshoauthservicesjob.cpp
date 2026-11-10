/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingsrefreshoauthservicesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SettingsRefreshOAuthServicesJob::SettingsRefreshOAuthServicesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SettingsRefreshOAuthServicesJob::~SettingsRefreshOAuthServicesJob() = default;

bool SettingsRefreshOAuthServicesJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SettingsRefreshOAuthServicesJob::start"_ba);
    submitPostRequest({});
    return true;
}

void SettingsRefreshOAuthServicesJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("SettingsRefreshOAuthServicesJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT settingsRefreshOAuthServicesDone(*replyObject);
    }
}

bool SettingsRefreshOAuthServicesJob::requireHttpAuthentication() const
{
    return true;
}

bool SettingsRefreshOAuthServicesJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest SettingsRefreshOAuthServicesJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SettingsRefreshOAuthServices);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool SettingsRefreshOAuthServicesJob::requireTwoFactorAuthentication() const
{
    return true;
}

#include "moc_settingsrefreshoauthservicesjob.cpp"
