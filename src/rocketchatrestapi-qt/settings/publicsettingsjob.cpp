/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "publicsettingsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
PublicSettingsJob::PublicSettingsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

PublicSettingsJob::~PublicSettingsJob() = default;

bool PublicSettingsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start PublicSettingsJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("PublicSettingsJob: Ask public info"));

    return true;
}

void PublicSettingsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("PublicSettingsJob done: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT publicSettingsDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("PublicSettingsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool PublicSettingsJob::requireHttpAuthentication() const
{
    return false;
}

QNetworkRequest PublicSettingsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SettingsPublic);
    QNetworkRequest req(url);
    addAuthRawHeader(req);
    return req;
}

#include "moc_publicsettingsjob.cpp"
