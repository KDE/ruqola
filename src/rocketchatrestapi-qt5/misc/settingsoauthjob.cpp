/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &SettingsOauthJob::slotSettingsOauthFinished);
    addStartRestApiInfo(QByteArrayLiteral("SettingsOauthJob: Ask settings oauth"));
    return true;
}

void SettingsOauthJob::slotSettingsOauthFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SettingsOauthJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT settingsOauthDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("SettingsOauthJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QNetworkRequest SettingsOauthJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SettingsOauth);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}
