/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingsremovecustomoauthjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SettingsRemoveCustomOAuthJob::SettingsRemoveCustomOAuthJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SettingsRemoveCustomOAuthJob::~SettingsRemoveCustomOAuthJob() = default;

bool SettingsRemoveCustomOAuthJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SettingsRemoveCustomOAuthJob::start"_ba);
    submitPostRequest(json());
    return true;
}

void SettingsRemoveCustomOAuthJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("SettingsRemoveCustomOAuthJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT settingsRemoveCustomOAuthDone(*replyObject);
    }
}

QString SettingsRemoveCustomOAuthJob::name() const
{
    return mName;
}

void SettingsRemoveCustomOAuthJob::setName(const QString &name)
{
    mName = name;
}

bool SettingsRemoveCustomOAuthJob::requireHttpAuthentication() const
{
    return true;
}

bool SettingsRemoveCustomOAuthJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SettingsRemoveCustomOAuthJob: name is empty";
        return false;
    }
    return true;
}

QNetworkRequest SettingsRemoveCustomOAuthJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SettingsRemoveCustomOAuth);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SettingsRemoveCustomOAuthJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["name"_L1] = mName;
    return QJsonDocument(jsonObj);
}

bool SettingsRemoveCustomOAuthJob::requireTwoFactorAuthentication() const
{
    return true;
}

#include "moc_settingsremovecustomoauthjob.cpp"
