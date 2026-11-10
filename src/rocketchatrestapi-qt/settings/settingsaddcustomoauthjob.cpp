/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingsaddcustomoauthjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SettingsAddCustomOAuthJob::SettingsAddCustomOAuthJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SettingsAddCustomOAuthJob::~SettingsAddCustomOAuthJob() = default;

bool SettingsAddCustomOAuthJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SettingsAddCustomOAuthJob::start"_ba);
    submitPostRequest(json());
    return true;
}

void SettingsAddCustomOAuthJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("SettingsAddCustomOAuthJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT settingsAddCustomOAuthDone(*replyObject);
    }
}

QString SettingsAddCustomOAuthJob::name() const
{
    return mName;
}

void SettingsAddCustomOAuthJob::setName(const QString &name)
{
    mName = name;
}

bool SettingsAddCustomOAuthJob::requireHttpAuthentication() const
{
    return true;
}

bool SettingsAddCustomOAuthJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SettingsAddCustomOAuthJob: name is empty";
        return false;
    }
    return true;
}

QNetworkRequest SettingsAddCustomOAuthJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SettingsAddCustomOAuth);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SettingsAddCustomOAuthJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["name"_L1] = mName;
    return QJsonDocument(jsonObj);
}

bool SettingsAddCustomOAuthJob::requireTwoFactorAuthentication() const
{
    return true;
}

#include "moc_settingsaddcustomoauthjob.cpp"
