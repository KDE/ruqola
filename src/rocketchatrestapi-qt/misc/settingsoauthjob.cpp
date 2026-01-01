/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingsoauthjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
SettingsOauthJob::SettingsOauthJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SettingsOauthJob::~SettingsOauthJob() = default;

bool SettingsOauthJob::requireHttpAuthentication() const
{
    return false;
}

bool SettingsOauthJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SettingsOauthJob";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("SettingsOauthJob: Ask settings oauth"_ba);
    return true;
}

void SettingsOauthJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SettingsOauthJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT settingsOauthDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SettingsOauthJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest SettingsOauthJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SettingsOauth);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_settingsoauthjob.cpp"
