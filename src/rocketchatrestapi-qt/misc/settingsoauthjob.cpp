/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingsoauthjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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

    addStartRestApiInfo(QByteArrayLiteral("SettingsOauthJob: Ask settings oauth"));
    return true;
}

void SettingsOauthJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("SettingsOauthJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT settingsOauthDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SettingsOauthJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest SettingsOauthJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SettingsOauth);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_settingsoauthjob.cpp"
